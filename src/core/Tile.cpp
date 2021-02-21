#include <Pch.h>

#include "Tile.h"

// clang-format off
const std::vector<unsigned int> Tile::_indices{0, 1, 2, 2, 3, 0};
const VertexLayout Tile::_layout{
    {3, GL_FLOAT},
    {1, GL_FLOAT}
};
// clang-format on

Tile::Tile() { initBuffers(0, 0, 0); }

Tile::Tile(int iX, int iY, int iIndex) { initBuffers(iX, iY, iIndex); }

void Tile::bind() const { _vb.bind(); }

void Tile::setIndex(int iIndex) {
    _vb.setVertexElement(0, 1, 0, static_cast<float>(iIndex));
    _vb.setVertexElement(1, 1, 0, static_cast<float>(iIndex));
    _vb.setVertexElement(2, 1, 0, static_cast<float>(iIndex));
    _vb.setVertexElement(3, 1, 0, static_cast<float>(iIndex));
}

void Tile::setPosition(float iX, float iY) {
    _vb.setVertexElement(0, 0, 0, iX + _vertices[0 + _layout.getCount() * 0]);
    _vb.setVertexElement(1, 0, 0, iX + _vertices[0 + _layout.getCount() * 1]);
    _vb.setVertexElement(2, 0, 0, iX + _vertices[0 + _layout.getCount() * 2]);
    _vb.setVertexElement(3, 0, 0, iX + _vertices[0 + _layout.getCount() * 3]);

    _vb.setVertexElement(0, 0, 1, iY + _vertices[1 + _layout.getCount() * 0]);
    _vb.setVertexElement(1, 0, 1, iY + _vertices[1 + _layout.getCount() * 1]);
    _vb.setVertexElement(2, 0, 1, iY + _vertices[1 + _layout.getCount() * 2]);
    _vb.setVertexElement(3, 0, 1, iY + _vertices[1 + _layout.getCount() * 3]);
}

void Tile::initBuffers(int iX, int iY, int iIndex) {
    _vb.setVertexBuffer(_vertices, sizeof(_vertices) / sizeof(float));
    _vb.setVertexLayout(_layout);
    _vb.setIndexBuffer(_indices);

    setPosition(iX, iY);
    setIndex(iIndex);
}