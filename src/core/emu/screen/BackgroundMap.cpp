#include <Pch.h>

#include <core/emu/screen/BackgroundMap.h>

BackgroundMap::BackgroundMap() {
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 32; ++x) {
            backgroundMap[x][y].setPosition(x - 16, 16 - y);
        }
    }
}

void BackgroundMap::reindex(const unsigned char* iBackgroundTileMap, bool iSignedIndexes) {
    int offset = iSignedIndexes ? 128 : 0;
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 32; ++x) {
            backgroundMap[x][y].setIndex((char)(iBackgroundTileMap[x + 32 * y]) + offset);
        }
    }
}

void BackgroundMap::render(const Renderer& renderer, const Video& iVideo, Program& ioProgram) const {
    iVideo.getBackgroundTableTexture()->bind(0);
    ioProgram.setUniform("u_BackgroundTableTexture", 0);
    ioProgram.setUniform("u_RelativeTileWidth", 1.0f / Video::kBackgroundTableSize);
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 32; ++x) {
            renderer.draw(backgroundMap[x][y].getVertexBuffer());
        }
    }
}
