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
    uint32_t tileIndex;

    static const VertexLayout& ToLayout() {
        static VertexLayout Layout{{3, GL_FLOAT}, {1, GL_UNSIGNED_INT}};
        return Layout;
    }
};

struct SpriteTileVertex {
    uint32_t spriteIndex;

    static const VertexLayout& ToLayout() {
        static VertexLayout Layout{{1, GL_UNSIGNED_INT}};
        return Layout;
    }
};

GLRenderer::GLRenderer()
    : _colorPalette({{0xD0, 0xE0, 0xF0, 0xFF},
                     {0x98, 0x98, 0x98, 0xFF},
                     {0x68, 0x68, 0x68, 0xFF},
                     {0x38, 0x38, 0x38, 0xFF}}),
      _obj0ColorPalette({{0xD0, 0xE0, 0xF0, 0x00},
                         {0x98, 0x98, 0x98, 0xFF},
                         {0x68, 0x68, 0x68, 0xFF},
                         {0x38, 0x38, 0x38, 0xFF}}),
      _obj1ColorPalette({{0xD0, 0xE0, 0xF0, 0x00},
                         {0x98, 0x98, 0x98, 0xFF},
                         {0x68, 0x68, 0x68, 0xFF},
                         {0x38, 0x38, 0x38, 0xFF}}),
      _scale(1.0f) {
    initBackround();
    initOam();

    _tileDecoderProgram = std::make_unique<GLProgram>();
    _renderProgram      = std::make_unique<GLProgram>();

    _tileDecoderProgram->loadShader("sandbox/compute.shader",
                                    GL_COMPUTE_SHADER);
    _tileDecoderProgram->link();
    _compressedTileData = std::make_unique<GLShaderStorageBuffer>(0);

    _renderProgram->loadShader("sandbox/vertex.shader", GL_VERTEX_SHADER);
    _renderProgram->loadShader("sandbox/fragment.shader", GL_FRAGMENT_SHADER);
    _renderProgram->link();

    _rendererShaderData = std::make_unique<GLShaderStorageBuffer>(0);
}

void GLRenderer::clear(float iRed, float iGreen, float iBlue,
                       float iAlpha) const {
    GLCall(glClearColor(iRed, iGreen, iBlue, iAlpha));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void GLRenderer::flush() const {}

void GLRenderer::setBackgroundTileData(void* iData, size_t iSize) {
    _compressedTileData->uploadData(iData, iSize, GL_STATIC_READ);
    _tileDecoderProgram->bind();
    _compressedTileData->bind();
    int textureBufferRef = 0;
    _tileDecoderProgram->setUniform("u_ImageOutput", textureBufferRef);
    _tileDecoderProgram->setUniformMatrix4("u_Palette", _colorPalette);

    _backgroundTileTexture->associateToWritableBuffer(0);

    _tileDecoderProgram->execute(256, 1, 1);
}

void GLRenderer::setBackgroundTileMapData(void* iData, size_t iSize) {
    _rendererShaderData->uploadData(iData, iSize, GL_STATIC_READ);
}

void GLRenderer::drawBackground() const {
    SynchronizeOn(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    int textureRef = _backgroundTileTexture->getTextureSlot();
    _renderProgram->bind();
    _rendererShaderData->bind();
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

void GLRenderer::setSignedBackgroundTileMap(bool iSigned) const {
    _renderProgram->bind();
    _renderProgram->setUniform("u_SignedTileIndexOffset", (iSigned ? 128 : 0));
    _renderProgram->unbind();
}

void GLRenderer::initBackround() {
    constexpr uint32_t kTilesPerSide    = 32;
    constexpr uint32_t kBackgroundTiles = kTilesPerSide * kTilesPerSide;

    _backgroundTileTexture =
        std::make_unique<GLTexture>(nullptr, 8, 8 * 256, 1, 4, 4);

    std::shared_ptr<GLVertexBuffer> vb =
        createTileGridVertexBuffer(kTilesPerSide, kBackgroundTiles);

    std::shared_ptr<IndexBuffer> ib =
        createTileGridIndexBuffer(kBackgroundTiles);

    _backgroundVertexArray.setIndexBuffer(ib);
    _backgroundVertexArray.addVertexBuffer(vb);
}
std::shared_ptr<GLVertexBuffer> GLRenderer::createTileGridVertexBuffer(
    const uint32_t iTilePerColumn, const uint32_t iTotalNumberOfTiles) const {
    const uint32_t totalBackgroundTilesVertices =
        iTotalNumberOfTiles * 4;  // 4 vertices per tile
    auto backgroundTiles =
        new BackgroundTileVertex[totalBackgroundTilesVertices];
    glm::vec2 positionPattern[4] = {
        // ortho projection has (0,0) on top left
        {0, 1},
        {1, 1},
        {1, 0},
        {0, 0},
    };
    for (uint32_t tileId = 0; tileId < iTotalNumberOfTiles; ++tileId) {
        for (uint32_t vertex = 0; vertex < 4; ++vertex) {
            backgroundTiles[tileId * 4 + vertex].tileIndex = tileId;
            backgroundTiles[tileId * 4 + vertex].position  = {
                (positionPattern[vertex].x + (tileId % iTilePerColumn)) * 8.0f,
                (positionPattern[vertex].y + (tileId / iTilePerColumn)) * 8.0f,
                0.0f};
        }
    }
    auto vb = std::make_shared<GLVertexBuffer>(
        backgroundTiles,
        sizeof(*backgroundTiles) * totalBackgroundTilesVertices);
    delete[] backgroundTiles;
    vb->setLayout(BackgroundTileVertex::ToLayout());
    return vb;
}

std::shared_ptr<IndexBuffer> GLRenderer::createTileGridIndexBuffer(
    const uint32_t iTotalNumberOfTiles) const {
    const uint32_t kBgIndices = iTotalNumberOfTiles * 6;  // 6 indices each tile
    auto indices              = new uint32_t[kBgIndices];
    uint32_t pattern[]        = {0, 1, 2, 2, 3, 0};
    for (int i = 0; i < kBgIndices; ++i) {
        indices[i] = ((i / 6) * 4) + pattern[i % 6];
    }
    auto ib = std::make_shared<IndexBuffer>(indices, kBgIndices);
    delete[] indices;
    return ib;
}

// ------------------- Sprites -------------------

void GLRenderer::setSpriteTileData(void* iData, size_t iSize) {
    _spriteCompressedTileData->uploadData(iData, iSize, GL_STATIC_READ);
    _spriteTileDecoderProgram->bind();
    _spriteCompressedTileData->bind();

    int textureWritableBufferRef = 1;
    _spriteTileDecoderProgram->setUniform("u_ImageOutput",
                                          textureWritableBufferRef);
    _spriteTileDecoderProgram->setUniformMatrix4("u_Palette",
                                                 _obj0ColorPalette);

    _spriteTileTexture->associateToWritableBuffer(textureWritableBufferRef);

    _spriteTileDecoderProgram->execute(256, 1, 1);
}

void GLRenderer::setOam(void* iData, size_t iSize) {
    _oam->uploadData(iData, iSize, GL_STATIC_READ);
}

void GLRenderer::drawSprites() const {
    SynchronizeOn(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    _spriteRenderProgram->bind();
    _oam->bind();
    _spriteTileTexture->bind();
    _spriteRenderProgram->setUniform(
        "u_Texture", static_cast<int>(_spriteTileTexture->getTextureSlot()));
    _spriteRenderProgram->setUniformMatrix4("u_Proj",
                                            &(getProjectionMatrix())[0][0]);
    _spriteRenderProgram->setUniformMatrix4("u_Scale", &(_scale)[0][0]);
    _spriteVertexArray.bind();
    GLCall(glDrawElements(GL_TRIANGLES,
                          _spriteVertexArray.getIndexBuffer()->getCount(),
                          GL_UNSIGNED_INT, nullptr));
}

std::shared_ptr<GLVertexBuffer> GLRenderer::createSpritesVertexBuffer(
    const uint32_t iTotalSprites) {
    uint32_t iTotalNumberOfVertices = iTotalSprites * 4;
    auto sprites                    = new uint32_t[iTotalNumberOfVertices];

    for (uint32_t tileId = 0; tileId < iTotalSprites; ++tileId) {
        for (uint32_t vertex = 0; vertex < 4; ++vertex) {
            sprites[tileId * 4 + vertex] = tileId;
        }
    }

    auto vb = std::make_shared<GLVertexBuffer>(
        sprites, sizeof(*sprites) * iTotalNumberOfVertices);
    vb->setLayout(SpriteTileVertex::ToLayout());
    delete[] sprites;
    return vb;
}

void GLRenderer::initOam() {
    constexpr uint32_t kTotalNumberOfSprites = 40;

    _spriteRenderProgram = std::make_unique<GLProgram>();
    _spriteRenderProgram->loadShader("sandbox/spriteVertex.shader",
                                     GL_VERTEX_SHADER);
    _spriteRenderProgram->loadShader("sandbox/spriteFragment.shader",
                                     GL_FRAGMENT_SHADER);
    _spriteRenderProgram->link();

    _spriteTileTexture =
        std::make_unique<GLTexture>(nullptr, 8, 8 * 256, 1, 4, 4);
    _spriteCompressedTileData = std::make_unique<GLShaderStorageBuffer>(0);
    _spriteTileDecoderProgram = std::make_unique<GLProgram>();
    _spriteTileDecoderProgram->loadShader("sandbox/compute.shader",
                                          GL_COMPUTE_SHADER);
    _spriteTileDecoderProgram->link();

    std::shared_ptr<GLVertexBuffer> vb =
        createSpritesVertexBuffer(kTotalNumberOfSprites);

    std::shared_ptr<IndexBuffer> ib =
        createTileGridIndexBuffer(kTotalNumberOfSprites);

    _spriteVertexArray.setIndexBuffer(ib);
    _spriteVertexArray.addVertexBuffer(vb);

    _oam = std::make_unique<GLShaderStorageBuffer>(0);
}
