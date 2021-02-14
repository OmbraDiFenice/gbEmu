#include <Pch.h>

#include "TileMapAdapter.h"

GLTexture TileMapAdapter::toTexture(unsigned char *iTileMap,
                                    unsigned int iWidth, unsigned int iHeight,
                                    unsigned int iTileWidth,
                                    unsigned int iTileHeight) {
    reorderData(iTileMap, iWidth, iHeight, iTileWidth, iTileHeight);
    verticalMirror(iTileMap, iWidth, iHeight);

    unsigned char tileData[iWidth * iHeight * _bytesPerColor];
    mapToRgb(iTileMap, tileData, iWidth, iHeight);
    return GLTexture(tileData, iWidth, iHeight, _bytesPerColor);
}

void TileMapAdapter::verticalMirror(unsigned char *ioBuffer,
                                    unsigned int iWidth, unsigned int iHeight) {
    unsigned char buf[iWidth];
    for (int i = 0; i < iHeight / 2; ++i) {
        memcpy(buf, &ioBuffer[i * iWidth], iWidth);
        memcpy(&ioBuffer[i * iWidth], &ioBuffer[(iHeight - 1 - i) * iWidth],
               iWidth);
        memcpy(&ioBuffer[(iHeight - 1 - i) * iWidth], buf, iWidth);
    }
}

void TileMapAdapter::mapToRgb(unsigned char *iBuffer, unsigned char *oRgbBuffer,
                              unsigned int iWidth, unsigned int iHeight) {
    for (int i = 0; i < iWidth * iHeight; ++i) {
        memcpy(&oRgbBuffer[i * _bytesPerColor], &_colorMap[iBuffer[i]],
               _bytesPerColor);
    }
}

void TileMapAdapter::reorderData(unsigned char *ioBuffer, unsigned int iWidth,
                                 unsigned int iHeight, unsigned int iTileWidth,
                                 unsigned int iTileHeight) {
    const int totalTileNumber = (iWidth * iHeight) / (iTileWidth * iTileHeight);

    unsigned char orig[iWidth * iHeight];
    memcpy(orig, ioBuffer, iWidth * iHeight);

    unsigned char row[iTileWidth];
    for (int i = 0; i < totalTileNumber; ++i) {
        for (int r = 0; r < iTileHeight; ++r) {
            memcpy(row, &orig[r * iTileWidth + i * iTileWidth * iTileHeight],
                   iTileWidth);
            memcpy(&ioBuffer[r * totalTileNumber * iTileWidth + i * iTileWidth],
                   row, iTileWidth);
        }
    }
}