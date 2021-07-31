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
            _spritePatternTable[x + 16 * y].setPosition(x, y);
        }
    }
}

void SpriteMap::update() {
    for (unsigned int spriteIndex = 0; spriteIndex < 40; ++spriteIndex) {
        const ObjectAttributeMemoryElement& spriteAttribute = _oam[spriteIndex];
        _spritesToBeDrawn[spriteIndex].setTileIndex(
            spriteAttribute.getPatternIndex());
    }
}

void SpriteMap::render(const GbRenderer& renderer) const {
    _program->bind();
    _spriteTableTexture->bind();

    glm::mat4 identity(1.0);

    // this scaling brings the size of the tile from 1x1 pixels to tileWidth x
    // tileHeight pixels
    glm::vec3 scalingVec(Video::kTileWidth, Video::kTileHeight, 1);
    glm::vec3 positionVec(0, 0, 0);

    for (unsigned int spriteIndex = 0; spriteIndex < 40; ++spriteIndex) {
        const ObjectAttributeMemoryElement& spriteAttribute = _oam[spriteIndex];
        positionVec.x = spriteAttribute.getX() - 8;
        positionVec.y = spriteAttribute.getY() - 16;

        if (spriteAttribute.getFlag(
                ObjectAttributeMemoryElement::Flag::yFlip)) {
            scalingVec.y *= -1;
        }
        if (spriteAttribute.getFlag(
                ObjectAttributeMemoryElement::Flag::xFlip)) {
            scalingVec.x *= -1;
        }

        glm::mat4 position = glm::translate(identity, positionVec);
        glm::mat4 scale    = glm::scale(identity, scalingVec);
        _program->setUniformMatrix4("u_Transform", &(position * scale)[0][0]);

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
