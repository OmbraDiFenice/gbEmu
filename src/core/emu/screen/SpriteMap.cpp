#include <Pch.h>

#include <core/emu/screen/SpriteMap.h>
#include <core/emu/screen/Video.h>
#include <core/ui/opengl/GLProgram.h>

SpriteMap::SpriteMap()
    : _program(nullptr),
      _colorMap({{0xD0, 0xE0, 0xF0, 0x00},
                 {0x98, 0x98, 0x98, 0xFF},
                 {0x68, 0x68, 0x68, 0xFF},
                 {0x38, 0x38, 0x38, 0xFF}}) {
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            _spritePatternTable[x + 16 * y].setTileIndex(x + 16 * y);
            _spritePatternTable[x + 16 * y].setPosition(x - 16, 16 - y);
        }
    }
}

void SpriteMap::update() {
    for (unsigned int spriteIndex = 0; spriteIndex < 40; ++spriteIndex) {
        const ObjectAttributeMemoryElement& spriteAttribute = _oam[spriteIndex];

        _spritesToBeDrawn[spriteIndex].setPosition(
            (spriteAttribute.getX() - 8) / 8 - 16,
            16 - (spriteAttribute.getY() - 16) / 8);

        _spritesToBeDrawn[spriteIndex].setTileIndex(
            spriteAttribute.getPatternIndex());
    }
}

void SpriteMap::render(const GbRenderer& renderer) const {
    _program->bind();
    _spriteTableTexture->bind();
    for (unsigned int spriteIndex = 0; spriteIndex < 40; ++spriteIndex) {
        renderer.draw(_spritesToBeDrawn[spriteIndex].getVertexBuffer());
    }
}

void SpriteMap::initialize() {
    _program = std::make_unique<GLProgram>();
    _program->loadShader("vertex.shader", GL_VERTEX_SHADER);
    _program->loadShader("fragment.shader", GL_FRAGMENT_SHADER);
    _program->link();
    _program->bind();

    _program->setUniform("u_TilePatterns", Video::TextureSlot::Sprites);
    _program->setUniform("u_TilemapHeight",
                         static_cast<float>(Video::kSpriteTableSize));
    _program->setUniformMatrix4("u_Palette", _colorMap);

    _program->unbind();
}
