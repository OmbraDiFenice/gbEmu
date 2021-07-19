#pragma once

#include <core/emu/screen/TilePatternAdapter.h>

class Video {
   public:
    static constexpr unsigned int kTileWidth    = 8;   // pixels
    static constexpr unsigned int kTileHeight   = 8;   // pixels
    static constexpr unsigned int kTileDataSize = 16;  // bytes, compressed

    static constexpr unsigned int kBackgroundTableSize = 256;  // tiles
    static constexpr unsigned int kSpriteTableSize     = 256;  // tiles
    static constexpr unsigned int kDecodedTileSize =
        kTileWidth * kTileHeight;  // bytes

    using CompressedTileData = unsigned char[kTileDataSize];
    using TileData           = unsigned char[kDecodedTileSize];

   public:
    explicit Video(TilePatternAdapter& adapter);
    void decodeTileMapPatterns(CompressedTileData* iBackgroundPatterns);
    std::shared_ptr<Texture> decodeSpritePatterns(
        CompressedTileData* iSpritePatterns);

    const std::shared_ptr<Texture>& getSpriteTableTexture() const {
        return _spriteTableTexture;
    }
    const std::shared_ptr<Texture>& getBackgroundTableTexture() const {
        return _backgroundTableTexture;
    }

   protected:
    void decodeTilePatterns(const CompressedTileData* iCompressedTilePatterns,
                            unsigned int iSize, TileData* oDecodedTilePatterns);

   private:
    void decodeTile(const CompressedTileData& iTileData,
                    TileData& oDecodedTile);

   private:
    TilePatternAdapter& _adapter;

    std::shared_ptr<Texture> _backgroundTableTexture;
    std::shared_ptr<Texture> _spriteTableTexture;
};
