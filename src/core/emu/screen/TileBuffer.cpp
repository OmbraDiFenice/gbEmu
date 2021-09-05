#include <Pch.h>

#include <core/emu/screen/TileBuffer.h>

TileBuffer::TileBuffer() { init(0, 0, 0); }

TileBuffer::TileBuffer(int iX, int iY, int iIndex) { init(iX, iY, iIndex); }

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

void TileBuffer::init(int iX, int iY, int iIndex) {
    setPosition(iX, iY);
    setTileIndex(iIndex);
}