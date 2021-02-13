#pragma once

class Video {
   public:
    static constexpr size_t kTileWidth         = 8;    // pixels
    static constexpr size_t kTileHeight        = 8;    // pixels
    static constexpr size_t kTileDataSize      = 16;   // bytes
    static constexpr size_t kTileDataTableSize = 256;  // tiles
    static constexpr size_t kDecodedTileSize =
        kTileWidth * kTileHeight;  // bytes
    static constexpr size_t kTileMapSize =
        Video::kTileDataTableSize * Video::kTileDataSize; // bytes

   public:
    unsigned char tileMap[Video::kTileDataTableSize][Video::kDecodedTileSize];

   public:
    void decodeTileMap(unsigned char* iTileDataMap);

   private:
    void decodeTile(unsigned char* iTileData, unsigned char* oDecodedTile);
};
