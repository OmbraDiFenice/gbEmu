#include <Pch.h>

#include <core/emu/screen/BackgroundMap.h>
#include <core/emu/screen/Video.h>
#include <core/ui/opengl/GLProgram.h>

BackgroundMap::BackgroundMap()
    : _program(nullptr),
      _colorMap({{0xD0, 0xE0, 0xF0, 0xFF},
                 {0x98, 0x98, 0x98, 0xFF},
                 {0x68, 0x68, 0x68, 0xFF},
                 {0x38, 0x38, 0x38, 0xFF}}) {
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 32; ++x) {
            _backgroundPatternTable[x + 32 * y].setPosition(x - 16, 16 - y);
        }
    }
}

void BackgroundMap::reindex(const unsigned char* iBackgroundTileMap,
                            bool iSignedIndexes) {
    int offset = iSignedIndexes ? 128 : 0;
    for (int patternIndex = 0; patternIndex < 32 * 32; ++patternIndex) {
        _backgroundPatternTable[patternIndex].setTileIndex(
            static_cast<char>(iBackgroundTileMap[patternIndex]) + offset);
    }
}

void BackgroundMap::render(const GbRenderer& renderer) const {
    _program->bind();
    _backgroundTableTexture->bind();
    for (int patternIndex = 0; patternIndex < 32 * 32; ++patternIndex) {
        renderer.draw(_backgroundPatternTable[patternIndex].getVertexBuffer());
    }
}

void BackgroundMap::initialize() {
    _program = std::make_unique<GLProgram>();
    _program->loadShader("vertex.shader", GL_VERTEX_SHADER);
    _program->loadShader("fragment.shader", GL_FRAGMENT_SHADER);
    _program->link();
    _program->bind();

    _program->setUniform("u_TilePatterns", Video::TextureSlot::Background);
    _program->setUniform("u_TilemapHeight",
                         static_cast<float>(Video::kBackgroundTableSize));
    _program->setUniformMatrix4("u_Palette", _colorMap);

    _program->unbind();
}
