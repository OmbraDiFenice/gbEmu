#include <Pch.h>

#include <core/emu/screen/TilePatternAdapter.h>
#include <core/ui/opengl/GLTexture.h>

std::shared_ptr<Texture> TilePatternAdapter::toTexture(unsigned char *iTileMap,
                                                       unsigned int iWidth,
                                                       unsigned int iHeight,
                                                       int iTextureSlot) {
    std::shared_ptr<unsigned char[]> tileData(
        new unsigned char[iWidth * iHeight]);
    memcpy(tileData.get(), iTileMap, sizeof(unsigned char) * iWidth * iHeight);
    return std::make_shared<GLTexture>(tileData, iWidth, iHeight, iTextureSlot,
                                       1, 1);
}
