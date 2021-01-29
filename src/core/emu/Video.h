#pragma once

class Video {
   public:
    void decodeTileMap(unsigned char* iTileDataMap,
                       unsigned char* oDecodedTileMap);
    void decodeTile(unsigned char* iTileData, unsigned char* oDecodedTile);

   public:
    static const size_t kTileDataSize = 16;        // bytes
    static const size_t kTileDataTableSize = 256;  // tiles
    static const size_t kDecodedTileSize = 64;     // bytes
};
