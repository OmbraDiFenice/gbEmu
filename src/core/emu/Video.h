#pragma once

#include <core/TileMapPatternAdapter.h>

class Video {
   public:
    static constexpr unsigned int kTileWidth         = 8;   // pixels
    static constexpr unsigned int kTileHeight        = 8;   // pixels
    static constexpr unsigned int kTilesPerRow       = 32;  // tiles
    static constexpr unsigned int kTilesPerColumn    = 32;  // tiles
    static constexpr unsigned int kTileDataSize      = 16;  // bytes, compressed
    static constexpr unsigned int kBackgroundTableSize = 256;  // tiles
    static constexpr unsigned int kDecodedTileSize =
        kTileWidth * kTileHeight;  // bytes
    static constexpr unsigned int kTileMapSize =
        Video::kBackgroundTableSize * Video::kTileDataSize;  // bytes

    static constexpr unsigned int kSpriteTableSize   = 256;  // tiles
    static constexpr unsigned int kSpriteDataSize = 16;  // bytes, compressed
    static constexpr unsigned int kDecodedSpriteSize = kTileWidth * kTileHeight;  // bytes

    struct CompressedSprite {
        unsigned char data[Video::kSpriteDataSize];
    };
    struct Sprite {
        unsigned char data[Video::kDecodedSpriteSize];
    };

   public:

   public:
    explicit Video(TileMapPatternAdapter& adapter);
    void decodeTileMapPatterns(unsigned char* iTileDataMap);
    std::shared_ptr<Texture> decodeSpritePatterns(CompressedSprite* iSpritePatterns);

    const std::shared_ptr<Texture>& getSpriteTableTexture() const { return _spriteTableTexture; }
    const std::shared_ptr<Texture>& getBackgroundTableTexture() const { return _backgroundTableTexture; }

   private:
    void decodeTile(const unsigned char* iTileData, unsigned char* oDecodedTile);

   private:
    TileMapPatternAdapter& _adapter;

    std::shared_ptr<Texture> _backgroundTableTexture;
    std::shared_ptr<Texture> _spriteTableTexture;
};
