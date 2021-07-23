#include <Pch.h>

#include <core/emu/screen/SpriteMap.h>
#include <core/emu/screen/Video.h>
#include <core/ui/opengl/GLProgram.h>

SpriteMap::SpriteMap() {
    _program = std::make_unique<GLProgram>();
    _program->loadShader("vertex.shader", GL_VERTEX_SHADER);
    _program->loadShader("fragment.shader", GL_FRAGMENT_SHADER);
    _program->link();
    _program->bind();

    _program->setUniform("u_TilePatterns", Video::TextureSlot::Sprites);
    _program->setUniform("u_TilemapWidth", static_cast<float>(Video::kSpriteTableSize));

    _program->unbind();

    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            spriteMap[x][y].setTileIndex(x + 16 * y);
            spriteMap[x][y].setPosition(x - 16, 16 - y);
        }
    }
}

void SpriteMap::update() {
    for(unsigned int spriteIndex = 0; spriteIndex < 40; ++spriteIndex) {
        _sprites[spriteIndex].setPosition((_oam[spriteIndex].getX() - 8) / 8 - 16, 16 - (_oam[spriteIndex].getY() - 16) / 8);
        //_sprites[spriteIndex].setPosition(_oam[spriteIndex].getX() - 16, 16 - _oam[spriteIndex].getY());
        _sprites[spriteIndex].setTileIndex(_oam[spriteIndex].getPatternIndex());
    }
}

void SpriteMap::render(const GbRenderer& renderer) const {
    _program->bind();
    _spriteTableTexture->bind();
    for(unsigned int spriteIndex = 0; spriteIndex < 40; ++spriteIndex) {
        renderer.draw(_sprites[spriteIndex].getVertexBuffer());
    }

    // for (int y = 0; y < 16; ++y) {
    //     for (int x = 0; x < 16; ++x) {
    //         renderer.draw(spriteMap[x][y].getVertexBuffer());
    //     }
    // }
}
