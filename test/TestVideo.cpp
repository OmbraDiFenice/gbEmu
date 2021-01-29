#include <core/emu/Video.h>
#include <gtest/gtest.h>

class TestVideo : public ::testing::Test {
   protected:
    unsigned char* testTileData;
    unsigned char* expectedTestTile;
    Video video;

    TestVideo() {
        testTileData = new unsigned char[16];
        // clang-format off
        unsigned char testTileDataTmp[] = {
            0x7C, 0x7C, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0xFE,
            0xC6, 0xC6, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0x00,
        };
        // clang-format on
        memcpy(testTileData, testTileDataTmp, 16);

        expectedTestTile = new unsigned char[64];
        // clang-format off
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
        memcpy(expectedTestTile, expectedTestTileTmp, 64);
    }

    virtual ~TestVideo() {
        delete[] testTileData;
        delete[] expectedTestTile;
    }

    void checkTile(const unsigned char* expectedTile,
                   const unsigned char* actualTile) {
        // 8x8 pixel tile
        for (int x = 0; x < 8; ++x) {
            for (int y = 0; y < 8; ++y) {
                EXPECT_EQ(expectedTile[x + 8 * y], actualTile[x + 8 * y])
                    << "pixel mismatch at x: " << x << " y: " << y;
            }
        }
    }
};

TEST_F(TestVideo, decodeTile) {
    unsigned char decodedTile[Video::kDecodedTileSize];

    video.decodeTile(testTileData, decodedTile);

    checkTile(expectedTestTile, decodedTile);
}

TEST_F(TestVideo, decodeTileMap) {
    const size_t kTileMapSize =
        Video::kTileDataTableSize * Video::kTileDataSize;

    unsigned char tileMap[kTileMapSize];
    unsigned char
        decodedTileMap[Video::kTileDataTableSize * Video::kDecodedTileSize];

    // test tile at the beginning of the tileMap
    memcpy(&tileMap[0], testTileData, Video::kTileDataSize);
    // test tile at the end of the tileMap
    memcpy(&tileMap[kTileMapSize - Video::kTileDataSize], testTileData,
           Video::kTileDataSize);

    video.decodeTileMap(tileMap, decodedTileMap);

    checkTile(expectedTestTile, &decodedTileMap[0 * Video::kDecodedTileSize]);
    checkTile(expectedTestTile, &decodedTileMap[255 * Video::kDecodedTileSize]);
}