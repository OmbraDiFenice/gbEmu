#include <Pch.h>

#include "Video.h"

#include <core/emu/utils.h>

Video::Video(TileMapPatternAdapter& adapter) : _adapter(adapter) {}

void Video::decodeTileMapPatterns(unsigned char* iTileDataMap) {
    unsigned char tileMap[Video::kBackgroundTableSize][Video::kDecodedTileSize];

    for (int tileIndex = 0; tileIndex < kBackgroundTableSize; ++tileIndex) {
        decodeTile(&iTileDataMap[tileIndex * kTileDataSize],
                   tileMap[tileIndex]);
    }

    _backgroundTableTexture = _adapter.toTexture(
        reinterpret_cast<unsigned char*>(tileMap),
        Video::kTileWidth * Video::kBackgroundTableSize, Video::kTileHeight,
        Video::kTileWidth, Video::kTileHeight);
}

std::shared_ptr<Texture> Video::decodeSpritePatterns(CompressedSprite* iSpritePatterns) {
    Sprite spriteMapData[Video::kSpriteTableSize];

    for (int spriteIndex = 0; spriteIndex < kSpriteTableSize; ++spriteIndex) {
        decodeTile(iSpritePatterns[spriteIndex].data,
                   spriteMapData[spriteIndex].data);
    }

    _spriteTableTexture = _adapter.toTexture(
        reinterpret_cast<unsigned char*>(spriteMapData),
        Video::kTileWidth * Video::kSpriteTableSize, Video::kTileHeight,
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