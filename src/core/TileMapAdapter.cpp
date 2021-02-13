#include <Pch.h>

#include "TileMapAdapter.h"

GLTexture TileMapAdapter::toTexture(unsigned char *iTileMap) {
    verticalMirror(iTileMap);
    unsigned char tileData[Video::kTileWidth * Video::kTileHeight * 4];
    mapToRgb(iTileMap, tileData);
    return GLTexture(tileData, Video::kTileWidth, Video::kTileHeight, 4);
}

void TileMapAdapter::verticalMirror(unsigned char *ioBuffer) {
    constexpr size_t width  = Video::kTileWidth;
    constexpr size_t height = Video::kTileHeight;

    unsigned char buf[width];
    for (int i = 0; i < height / 2; ++i) {
        memcpy(buf, &ioBuffer[i * width], width);
        memcpy(&ioBuffer[i * width], &ioBuffer[(height - 1 - i) * width],
               width);
        memcpy(&ioBuffer[(height - 1 - i) * width], buf, width);
    }
}

void TileMapAdapter::mapToRgb(unsigned char *iBuffer,
                              unsigned char *oRgbBuffer) {
    constexpr size_t width  = Video::kTileWidth;
    constexpr size_t height = Video::kTileHeight;

    for (int i = 0; i < width * height; ++i) {
        memcpy(&oRgbBuffer[i * 4], &_colorMap[iBuffer[i]], 4);
    }
}
