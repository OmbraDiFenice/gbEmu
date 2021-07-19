#include <Pch.h>

#include "Video.h"

#include <core/emu/utils.h>

Video::Video(TilePatternAdapter& adapter) : _adapter(adapter) {}

void Video::decodeTileMapPatterns(CompressedTileData* iBackgroundPatterns) {
    TileData tileMapData[kBackgroundTableSize];

    decodeTilePatterns(iBackgroundPatterns, kBackgroundTableSize, tileMapData);

    _backgroundTableTexture =
        _adapter.toTexture(reinterpret_cast<unsigned char*>(tileMapData),
                           kTileWidth * kBackgroundTableSize, kTileHeight,
                           kTileWidth, kTileHeight);
}

std::shared_ptr<Texture> Video::decodeSpritePatterns(
    CompressedTileData* iSpritePatterns) {
    TileData spriteMapData[kSpriteTableSize];

    decodeTilePatterns(iSpritePatterns, kSpriteTableSize, spriteMapData);

    _spriteTableTexture = _adapter.toTexture(
        reinterpret_cast<unsigned char*>(spriteMapData),
        kTileWidth * kSpriteTableSize, kTileHeight, kTileWidth, kTileHeight);

    return _spriteTableTexture;
}

void Video::decodeTilePatterns(
    const CompressedTileData* iCompressedTilePatterns, const unsigned int iSize,
    TileData* oDecodedTilePatterns) {
    for (int tileIndex = 0; tileIndex < iSize; ++tileIndex) {
        decodeTile(iCompressedTilePatterns[tileIndex],
                   oDecodedTilePatterns[tileIndex]);
    }
}

void Video::decodeTile(const CompressedTileData& iTileData,
                       TileData& oDecodedTile) {
    for (int row = 0; row < kTileDataSize; row += 2) {  // 2 bytes per pixel row
        unsigned char lsbyte = iTileData[row];
        unsigned char msbyte = iTileData[row + 1];

        for (int col = 0; col < 8; ++col) {  // loop over bits in each byte
            oDecodedTile[col + 8 * (row / 2)] =
                (BIT(msbyte, 7 - col) << 1) | BIT(lsbyte, 7 - col);
        }
    }
}