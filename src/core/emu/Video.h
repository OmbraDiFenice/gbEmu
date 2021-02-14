#pragma once

class Video {
   public:
    static constexpr unsigned int kTileWidth         = 8;    // pixels
    static constexpr unsigned int kTileHeight        = 8;    // pixels
    static constexpr unsigned int kTileDataSize      = 16;   // bytes
    static constexpr unsigned int kTileDataTableSize = 256;  // tiles
    static constexpr unsigned int kDecodedTileSize =
        kTileWidth * kTileHeight;  // bytes
    static constexpr unsigned int kTileMapSize =
        Video::kTileDataTableSize * Video::kTileDataSize;  // bytes

   public:
    unsigned char tileMap[Video::kTileDataTableSize][Video::kDecodedTileSize];

   public:
    void decodeTileMap(unsigned char* iTileDataMap);

   private:
    void decodeTile(unsigned char* iTileData, unsigned char* oDecodedTile);
};
