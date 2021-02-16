#include <Pch.h>

#include "Tile.h"

#include <core/emu/Video.h>
#include <glad/gl.h>
#include <utils/GLErrorMacros.h>

Tile::Tile(int iX, int iY, const int iIndex) {
    setPosition(iX, iY);
    setIndex(iIndex);
}

void Tile::bind() const {
    /* vertex array */
    GLuint vertexArray;
    GLCall(glGenVertexArrays(1, &vertexArray));
    GLCall(glBindVertexArray(vertexArray));

    /* vertex buffer */
    GLuint vertexBuffer;
    GLCall(glGenBuffers(1, &vertexBuffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                        GL_STATIC_DRAW));

    /* index buffer */
    GLuint indexBuffer;
    GLCall(glGenBuffers(1, &indexBuffer));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                        GL_STATIC_DRAW));

    /* vertex array attrib */
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                                 nullptr));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                                 (const void*)(3 * sizeof(float))));
}

void Tile::setIndex(const int iIndex) {
    _index = iIndex;
    setTextureCoords();
}

void Tile::setTextureCoords() {
    constexpr float relativeTileWidth = 1.0f / Video::kTileDataTableSize;
    vertices[3 + 5 * 0] =  _index * relativeTileWidth;
    vertices[3 + 5 * 1] =  (_index + 1) * relativeTileWidth;
    vertices[3 + 5 * 2] =  (_index + 1) * relativeTileWidth;
    vertices[3 + 5 * 3] =  _index * relativeTileWidth;
    bind();
}

void Tile::setPosition(float iX, float iY) {
    vertices[0 + 5 * 0] = iX + vertices[0 + 5 * 0];
    vertices[0 + 5 * 1] = iX + vertices[0 + 5 * 1];
    vertices[0 + 5 * 2] = iX + vertices[0 + 5 * 2];
    vertices[0 + 5 * 3] = iX + vertices[0 + 5 * 3];

    vertices[1 + 5 * 0] = iY + vertices[1 + 5 * 0];
    vertices[1 + 5 * 1] = iY + vertices[1 + 5 * 1];
    vertices[1 + 5 * 2] = iY + vertices[1 + 5 * 2];
    vertices[1 + 5 * 3] = iY + vertices[1 + 5 * 3];
}
