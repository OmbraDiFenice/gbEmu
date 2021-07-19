#include <Pch.h>

#include <core/emu/screen/SpriteMap.h>

SpriteMap::SpriteMap() {
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            spriteMap[x][y].setIndex(x + 16 * y);
            spriteMap[x][y].setPosition(x - 16, 16 - y);
        }
    }
}

void SpriteMap::render(const Renderer& renderer, const Video& iVideo,
                       Program& ioProgram) const {
    iVideo.getSpriteTableTexture()->bind(1);
    ioProgram.setUniform("u_TileTexture", 1);
    ioProgram.setUniform("u_RelativeTileWidth", 1.0f / Video::kSpriteTableSize);
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            renderer.draw(spriteMap[x][y].getVertexBuffer());
        }
    }
}
