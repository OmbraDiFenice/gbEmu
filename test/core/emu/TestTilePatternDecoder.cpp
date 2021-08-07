#include <gtest/gtest.h>

#include <core/emu/screen/TilePatternAdapter.h>
#include <core/emu/screen/Video.h>

class TestTilePatternDecoder : public ::testing::Test {
   protected:
    Video::TilePatternDecoder decoder;

    Video::CompressedTileData testTileData;
    Video::TileData expectedTestTile;

    TestTilePatternDecoder() {
        // clang-format off
        Video::CompressedTileData testTileDataTmp = {
            0x7C, 0x7C, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0xFE,
            0xC6, 0xC6, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0x00,
        };

        Video::TileData expectedTestTileTmp = {
            0, 3, 3, 3, 3, 3, 0, 0,
            2, 2, 0, 0, 0, 2, 2, 0,
            1, 1, 0, 0, 0, 1, 1, 0,
            2, 2, 2, 2, 2, 2, 2, 0,
            3, 3, 0, 0, 0, 3, 3, 0,
            2, 2, 0, 0, 0, 2, 2, 0,
            1, 1, 0, 0, 0, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0
        };
        // clang-format on

        memcpy(testTileData, testTileDataTmp, 16);
        memcpy(expectedTestTile, expectedTestTileTmp, 64);
    }

    void checkTile(unsigned char* expectedTile, unsigned char* actualTile,
                   const char* message) {
        // 8x8 pixel tile
        for (int x = 0; x < 8; ++x) {
            for (int y = 0; y < 8; ++y) {
                EXPECT_EQ(expectedTile[x + 8 * y], actualTile[x + 8 * y])
                    << message << ": pixel mismatch at x= " << x << " y= " << y;
            }
        }
    }
};

TEST_F(TestTilePatternDecoder, decodeTilePatterns) {
    Video::CompressedTileData tileMapData[Video::kBackgroundTableSize];
    Video::TileData uncompressedTileMapData[Video::kBackgroundTableSize];

    // set tile at the beginning of the tileMapData
    memcpy(&tileMapData[0], testTileData, Video::kTileDataSize);

    // set tile at the end of the tileMapData
    memcpy(&tileMapData[Video::kBackgroundTableSize - 1], testTileData,
           Video::kTileDataSize);

    decoder.decodeTilePatterns(tileMapData, Video::kBackgroundTableSize,
                               uncompressedTileMapData);

    checkTile(expectedTestTile, uncompressedTileMapData[0], "first tile");
    checkTile(expectedTestTile,
              uncompressedTileMapData[Video::kBackgroundTableSize - 1],
              "last tile");
}