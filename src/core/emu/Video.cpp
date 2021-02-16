#include <Pch.h>

#include "Video.h"

#include <core/emu/utils.h>

void Video::decodeTileMapPatterns(unsigned char* iTileDataMap) {
    for (int tileIndex = 0; tileIndex < kTileDataTableSize; ++tileIndex) {
        decodeTile(&iTileDataMap[tileIndex * kTileDataSize],
                   tileMap[tileIndex]);
    }
}

void Video::decodeTile(unsigned char* iTileData, unsigned char* oDecodedTile) {
    for (int row = 0; row < kTileDataSize; row += 2) {  // 2 bytes per pixel row
        unsigned char lsbyte = iTileData[row];
        unsigned char msbyte = iTileData[row + 1];

        for (int col = 0; col < 8; ++col) {  // loop over bits in each byte
            oDecodedTile[col + 8 * (row / 2)] =
                (BIT(msbyte, 7 - col) << 1) | BIT(lsbyte, 7 - col);
        }
    }
}