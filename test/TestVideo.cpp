#include <gtest/gtest.h>

#include <core/emu/Video.h>

class TestVideo : public ::testing::Test {
   protected:
    unsigned char testTileData[Video::kTileDataSize];
    unsigned char expectedTestTile[Video::kDecodedTileSize];
    Video video;

    TestVideo() {
        // clang-format off
        unsigned char testTileDataTmp[] = {
            0x7C, 0x7C, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0xFE,
            0xC6, 0xC6, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0x00,
        };

        unsigned char expectedTestTileTmp[] = {
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

    void checkTile(unsigned char* expectedTile, unsigned char* actualTile) {
        // 8x8 pixel tile
        for (int x = 0; x < 8; ++x) {
            for (int y = 0; y < 8; ++y) {
                EXPECT_EQ(expectedTile[x + 8 * y], actualTile[x + 8 * y])
                    << "pixel mismatch at x: " << x << " y: " << y;
            }
        }
    }
};

TEST_F(TestVideo, decodeTileMap) {
    unsigned char tileMap[Video::kTileMapSize];

    // set tile at the beginning of the tileMap
    memcpy(&tileMap[0], testTileData, Video::kTileDataSize);

    // set tile at the end of the tileMap
    memcpy(&tileMap[Video::kTileMapSize - Video::kTileDataSize], testTileData,
           Video::kTileDataSize);

    video.decodeTileMap(tileMap);

    checkTile(expectedTestTile, video.tileMap[0]);
    checkTile(expectedTestTile, video.tileMap[255]);
}