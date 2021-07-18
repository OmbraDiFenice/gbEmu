#include <Pch.h>

#include "Video.h"

#include <core/emu/utils.h>

Video::Video(TileMapPatternAdapter& adapter) : _adapter(adapter) {}

void Video::decodeTileMapPatterns(unsigned char* iTileDataMap) {
    for (int tileIndex = 0; tileIndex < kTileDataTableSize; ++tileIndex) {
        decodeTile(&iTileDataMap[tileIndex * kTileDataSize],
                   tileMap[tileIndex]);
    }
}

std::shared_ptr<Texture> Video::decodeSpritePatterns(CompressedSprite* iSpritePatterns) {
    Sprite spriteMapData[Video::kSpritePatternTableSize];

    for (int spriteIndex = 0; spriteIndex < kSpritePatternTableSize; ++spriteIndex) {
        decodeTile(iSpritePatterns[spriteIndex].data,
                   spriteMapData[spriteIndex].data);
    }

    _spriteTableTexture = _adapter.toTexture(
        reinterpret_cast<unsigned char*>(spriteMapData),
        Video::kTileWidth * Video::kSpritePatternTableSize, Video::kTileHeight,
        Video::kTileWidth, Video::kTileHeight);

    return _spriteTableTexture;
}

void Video::decodeTile(const unsigned char* iTileData, unsigned char* oDecodedTile) {
    for (int row = 0; row < kTileDataSize; row += 2) {  // 2 bytes per pixel row
        unsigned char lsbyte = iTileData[row];
        unsigned char msbyte = iTileData[row + 1];

        for (int col = 0; col < 8; ++col) {  // loop over bits in each byte
            oDecodedTile[col + 8 * (row / 2)] =
                (BIT(msbyte, 7 - col) << 1) | BIT(lsbyte, 7 - col);
        }
    }
}