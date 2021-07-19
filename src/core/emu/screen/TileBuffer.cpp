#include <Pch.h>

#include <core/emu/screen/TileBuffer.h>
#include <core/ui/opengl/GLVertexBuffer.h>

// clang-format off
const std::vector<unsigned int> TileBuffer::_indices{0, 1, 2, 2, 3, 0};
const VertexLayout TileBuffer::_layout{
    {3, GL_FLOAT},
    {1, GL_FLOAT}
};
// clang-format on

TileBuffer::TileBuffer() { initBuffers(0, 0, 0); }

TileBuffer::TileBuffer(int iX, int iY, int iIndex) { initBuffers(iX, iY, iIndex); }

void TileBuffer::bind() const { _vb->bind(); }

void TileBuffer::setIndex(int iIndex) {
    _vb->setVertexElement(0, 1, 0, static_cast<float>(iIndex));
    _vb->setVertexElement(1, 1, 0, static_cast<float>(iIndex));
    _vb->setVertexElement(2, 1, 0, static_cast<float>(iIndex));
    _vb->setVertexElement(3, 1, 0, static_cast<float>(iIndex));
}

void TileBuffer::setPosition(float iX, float iY) {
    _vb->setVertexElement(0, 0, 0, iX + _vertices[0 + _layout.getCount() * 0]);
    _vb->setVertexElement(1, 0, 0, iX + _vertices[0 + _layout.getCount() * 1]);
    _vb->setVertexElement(2, 0, 0, iX + _vertices[0 + _layout.getCount() * 2]);
    _vb->setVertexElement(3, 0, 0, iX + _vertices[0 + _layout.getCount() * 3]);

    _vb->setVertexElement(0, 0, 1, iY + _vertices[1 + _layout.getCount() * 0]);
    _vb->setVertexElement(1, 0, 1, iY + _vertices[1 + _layout.getCount() * 1]);
    _vb->setVertexElement(2, 0, 1, iY + _vertices[1 + _layout.getCount() * 2]);
    _vb->setVertexElement(3, 0, 1, iY + _vertices[1 + _layout.getCount() * 3]);
}

void TileBuffer::initBuffers(int iX, int iY, int iIndex) {
    _vb = std::make_shared<GLVertexBuffer>();
    _vb->setVertexBuffer(_vertices, 4 * _layout.getCount());
    _vb->setVertexLayout(_layout);
    _vb->setIndexBuffer(_indices);

    setPosition(iX, iY);
    setIndex(iIndex);
}