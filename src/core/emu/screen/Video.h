#pragma once

#include <core/emu/screen/TilePatternAdapter.h>
#include <core/emu/screen/BackgroundMap.h>
#include <core/emu/screen/SpriteMap.h>

class Video : public VideoItem {
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

    enum TextureSlot {
        Transparent = 0,
        Background = 1,
        Sprites = 2
    };

   public:
    explicit Video(TilePatternAdapter& adapter);

    /** Take data from gb video memory and rebuild background and sprite textures */
    void update();

    void render(const GbRenderer& renderer) const;

    const BackgroundMap& getBackground() const { return _background; };
    const SpriteMap& getSprites() const { return _sprites; };

   protected:
    void decodeTilePatterns(const CompressedTileData* iCompressedTilePatterns,
                            unsigned int iSize, TileData* oDecodedTilePatterns);

   private:
    std::shared_ptr<Texture> decodeTileMapPatterns(CompressedTileData* iBackgroundPatterns);
    std::shared_ptr<Texture> decodeSpritePatterns(
        CompressedTileData* iSpritePatterns);
    void decodeTile(const CompressedTileData& iTileData,
                    TileData& oDecodedTile);

   private:
    TilePatternAdapter& _adapter;

    BackgroundMap _background;
    SpriteMap _sprites;
};
