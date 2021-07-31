#include <Pch.h>

#include <core/emu/screen/TilePatternAdapter.h>
#include <core/ui/opengl/GLTexture.h>

std::shared_ptr<Texture> TilePatternAdapter::toTexture(unsigned char *iTileMap,
                                                       unsigned int iWidth,
                                                       unsigned int iHeight,
                                                       int iTextureSlot) {
    std::shared_ptr<unsigned char[]> tileData(
        new unsigned char[iWidth * iHeight * _bytesPerColor]);
    mapToRgb(iTileMap, tileData.get(), iWidth, iHeight);
    return std::make_shared<GLTexture>(tileData, iWidth, iHeight, iTextureSlot,
                                       _bytesPerColor);
}

void TilePatternAdapter::mapToRgb(const unsigned char *iBuffer,
                                     unsigned char *oRgbBuffer,
                                     unsigned int iWidth,
                                     unsigned int iHeight) {
    uint32_t filteredColorMap[4];
    memcpy(filteredColorMap, _colorMap, _bytesPerColor * 4);
    if(_transparentColorIndex >= 0) {
        filteredColorMap[_transparentColorIndex] &= 0x00FFFFFF;
    }

    for (int i = 0; i < iWidth * iHeight; ++i) {
        memcpy(&oRgbBuffer[i * _bytesPerColor], &filteredColorMap[iBuffer[i]],
               _bytesPerColor);
    }
}
