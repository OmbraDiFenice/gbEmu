#include <Pch.h>

#include <core/ui/opengl/GLProgram.h>
#include <core/ui/opengl/GLRenderer.h>
#include <core/ui/opengl/GLShaderStorageBuffer.h>
#include <core/ui/opengl/GLTexture.h>
#include <core/ui/opengl/Synchronization.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <utils/GLErrorMacros.h>

struct BackgroundTileVertex {
    glm::vec3 position;
    float tileId;

    static const VertexLayout& ToLayout() {
        static VertexLayout Layout{{3, GL_FLOAT}, {1, GL_FLOAT}};
        return Layout;
    }
};

GLRenderer::GLRenderer()
    : _backgroundDataDirty(false),
      _colorPalette({{0xD0, 0xE0, 0xF0, 0xFF},
                     {0x98, 0x98, 0x98, 0xFF},
                     {0x68, 0x68, 0x68, 0xFF},
                     {0x38, 0x38, 0x38, 0xFF}}),
      _scale(1.0f) {
    initBackround();

    _tileDecoderProgram = std::make_unique<GLProgram>();
    _renderProgram      = std::make_unique<GLProgram>();

    _tileDecoderProgram->loadShader("sandbox/compute.shader",
                                    GL_COMPUTE_SHADER);
    _tileDecoderProgram->link();
    _compressedTileData = std::make_unique<GLShaderStorageBuffer>();

    _renderProgram->loadShader("sandbox/vertex.shader", GL_VERTEX_SHADER);
    _renderProgram->loadShader("sandbox/fragment.shader", GL_FRAGMENT_SHADER);
    _renderProgram->link();
}

void GLRenderer::clear(float iRed, float iGreen, float iBlue,
                       float iAlpha) const {
    GLCall(glClearColor(iRed, iGreen, iBlue, iAlpha));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void GLRenderer::flush() const {}

void GLRenderer::setBackgroundTileData(void* iData, size_t iSize) {
    _compressedTileData->uploadData(iData, iSize, GL_STATIC_READ);
    _backgroundDataDirty = true;
}

void GLRenderer::setBackgroundTileMapData(void* iData, size_t iSize) {
    _backgroundVertexArray.getVertexBuffers().front()->setData(iData, iSize);
    _backgroundDataDirty = true;
}

void GLRenderer::drawBackground() const {
    if (_backgroundDataDirty) {
        _tileDecoderProgram->bind();
        _compressedTileData->bind();
        int textureBufferRef = 0;
        _tileDecoderProgram->setUniform("u_ImageOutput", textureBufferRef);
        _tileDecoderProgram->setUniformMatrix4("u_Palette", _colorPalette);

        _backgroundTileTexture->associateToWritableBuffer(0);

        _tileDecoderProgram->execute(256, 1, 1);
        SynchronizeOn(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        _backgroundDataDirty = false;
    }

    int textureRef = _backgroundTileTexture->getTextureSlot();
    _renderProgram->bind();
    _renderProgram->setUniform("u_Texture", textureRef);
    _renderProgram->setUniformMatrix4("u_Proj", &(getProjectionMatrix())[0][0]);
    _renderProgram->setUniformMatrix4("u_Scale", &(_scale)[0][0]);
    _backgroundTileTexture->bind();
    _backgroundVertexArray.bind();
    GLCall(glDrawElements(GL_TRIANGLES,
                          _backgroundVertexArray.getIndexBuffer()->getCount(),
                          GL_UNSIGNED_INT, nullptr));
}

const glm::mat4& GLRenderer::getProjectionMatrix() const {
    static glm::mat4 projectionMatrix = glm::ortho(0.0f, 160.0f, 144.0f, 0.0f);
    return projectionMatrix;
}

void GLRenderer::setScale(float iScale) {
    _scale = glm::scale(glm::mat4(1.0f), glm::vec3(iScale, iScale, 1));
}

void GLRenderer::initBackround() {
    constexpr uint32_t kTilesPerSide    = 32;
    constexpr uint32_t kBackgroundTiles = kTilesPerSide * kTilesPerSide;

    _backgroundTileTexture =
        std::make_unique<GLTexture>(nullptr, 8, 8 * 256, 1, 4, 4);

    BackgroundTileVertex
        backgroundTiles[kBackgroundTiles * 4];  // 4 vertices per tile
    glm::vec2 positionPattern[4] = {
        // ortho projection has (0,0) on top left
        {0, 1},
        {1, 1},
        {1, 0},
        {0, 0},
    };
    for (uint32_t tileId = 0; tileId < kBackgroundTiles; ++tileId) {
        for (uint32_t vertex = 0; vertex < 4; ++vertex) {
            backgroundTiles[tileId * 4 + vertex].tileId   = tileId;
            backgroundTiles[tileId * 4 + vertex].position = {
                (positionPattern[vertex].x + (tileId % kTilesPerSide)) * 8.0f,
                (positionPattern[vertex].y + (tileId / kTilesPerSide)) * 8.0f,
                0.0f};
        }
    }
    auto vb = std::make_shared<GLVertexBuffer>(backgroundTiles,
                                               sizeof(backgroundTiles));
    vb->setLayout(BackgroundTileVertex::ToLayout());

    const uint32_t kBgIndices = kBackgroundTiles * 6;  // 6 indices each tile
    uint32_t indices[kBgIndices];
    uint32_t pattern[] = {0, 1, 2, 2, 3, 0};
    for (int i = 0; i < kBgIndices; ++i) {
        indices[i] = ((i / 6) * 4) + pattern[i % 6];
    }
    auto ib = std::make_shared<IndexBuffer>(indices, kBgIndices);

    _backgroundVertexArray.setIndexBuffer(ib);
    _backgroundVertexArray.addVertexBuffer(vb);
}
