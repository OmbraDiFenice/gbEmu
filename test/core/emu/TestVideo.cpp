#include <gtest/gtest.h>

#include <core/emu/screen/TilePatternAdapter.h>
#include <core/emu/screen/Video.h>

class TestVideo : public Video, public ::testing::Test {
   protected:
    static TilePatternAdapter tileMapPatternAdapter;
    CompressedTileData testTileData;
    TileData expectedTestTile;

    TestVideo() : Video(tileMapPatternAdapter) {
        // clang-format off
        CompressedTileData testTileDataTmp = {
            0x7C, 0x7C, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0xFE,
            0xC6, 0xC6, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0x00,
        };

        TileData expectedTestTileTmp = {
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

TilePatternAdapter TestVideo::tileMapPatternAdapter;

TEST_F(TestVideo, decodeTilePatterns) {
    CompressedTileData tileMapData[kBackgroundTableSize];
    TileData uncompressedTileMapData[kBackgroundTableSize];

    // set tile at the beginning of the tileMapData
    memcpy(&tileMapData[0], testTileData, kTileDataSize);

    // set tile at the end of the tileMapData
    memcpy(&tileMapData[kBackgroundTableSize - 1], testTileData, kTileDataSize);

    decodeTilePatterns(tileMapData, kBackgroundTableSize,
                       uncompressedTileMapData);

    checkTile(expectedTestTile, uncompressedTileMapData[0], "first tile");
    checkTile(expectedTestTile,
              uncompressedTileMapData[kBackgroundTableSize - 1], "last tile");
}