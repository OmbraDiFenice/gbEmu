#include <Pch.h>

#include <core/emu/screen/TileBuffer.h>
#include <core/ui/opengl/GLVertexBuffer.h>

// clang-format off
const std::vector<unsigned int> TileBuffer::_indices{0, 1, 2, 2, 3, 0};
const VertexLayout TileBuffer::_layout{
    {3, GL_FLOAT},  // coords
    {1, GL_FLOAT}   // tile id
};
// clang-format on

TileBuffer::TileBuffer() { initBuffers(0, 0, 0); }

TileBuffer::TileBuffer(int iX, int iY, int iIndex) {
    initBuffers(iX, iY, iIndex);
}

void TileBuffer::bind() const { _vb->bind(); }

void TileBuffer::setPosition(float iX, float iY) {
    _vertices[0].position = glm::vec3(iX + 0.0f, iY + 0.0f, 0.0f);
    _vertices[1].position = glm::vec3(iX + 1.0f, iY + 0.0f, 0.0f);
    _vertices[2].position = glm::vec3(iX + 1.0f, iY + 1.0f, 0.0f);
    _vertices[3].position = glm::vec3(iX + 0.0f, iY + 1.0f, 0.0f);
}

void TileBuffer::setTileIndex(int iIndex) {
    _vertices[0].tileId = iIndex;
    _vertices[1].tileId = iIndex;
    _vertices[2].tileId = iIndex;
    _vertices[3].tileId = iIndex;
}

void TileBuffer::initBuffers(int iX, int iY, int iIndex) {
    ASSERT(_layout.getStride() == sizeof(TileVertex),
           "TileVertex struct needs to have the same size as a single vertex");

    setPosition(iX, iY);
    setTileIndex(iIndex);

    _vb = std::make_shared<GLVertexBuffer>();
    _vb->setVertexBuffer((float*)(&_vertices[0]),
                         vertexCount * sizeof(TileVertex));
    _vb->setVertexLayout(_layout);
    _vb->setIndexBuffer(_indices);
}