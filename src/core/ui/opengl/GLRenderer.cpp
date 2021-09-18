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

std::shared_ptr<IndexBuffer> generateTileSequenceIndexBuffer(
    const uint32_t iTotalNumberOfTiles) {
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
    _background.renderProgram = std::make_unique<GLProgram>();

    _background.renderProgram->loadShader("backgroundVertex.shader",
                                          GL_VERTEX_SHADER);
    _background.renderProgram->loadShader("fragment.shader",
                                          GL_FRAGMENT_SHADER);
    _background.renderProgram->link();

    _background.renderShaderData = std::make_unique<GLShaderStorageBuffer>(0);
}

void GLRenderer::clear(float iRed, float iGreen, float iBlue,
                       float iAlpha) const {
    GLCall(glClearColor(iRed, iGreen, iBlue, iAlpha));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void GLRenderer::flush() const {}

void GLRenderer::setBackgroundTileData(void* iData, size_t iSize) {
    _tileDecoder.decode(iData, iSize, _background.texture, _colorPalette);
}

void GLRenderer::setBackgroundTileMapData(void* iData, size_t iSize) {
    _background.renderShaderData->uploadData(iData, iSize, GL_STATIC_READ);
}

void GLRenderer::drawBackground() const {
    SynchronizeOn(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    int textureRef = _background.texture->getTextureSlot();
    _background.renderProgram->bind();
    _background.renderShaderData->bind();
    _background.renderProgram->setUniform("u_Texture", textureRef);
    _background.renderProgram->setUniformMatrix4(
        "u_Proj", &(getProjectionMatrix())[0][0]);
    _background.renderProgram->setUniformMatrix4("u_Scale", &(_scale)[0][0]);
    _background.texture->bind();
    _background.vertexArray.bind();
    GLCall(glDrawElements(GL_TRIANGLES,
                          _background.vertexArray.getIndexBuffer()->getCount(),
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
    _background.renderProgram->bind();
    _background.renderProgram->setUniform("u_SignedTileIndexOffset",
                                          (iSigned ? 128 : 0));
    _background.renderProgram->unbind();
}

void GLRenderer::setSpriteTileData(void* iData, size_t iSize) {
    _tileDecoder.decode(iData, iSize, _sprite.texture, _obj0ColorPalette);
}

void GLRenderer::setOam(void* iData, size_t iSize) {
    _sprite.oam->uploadData(iData, iSize, GL_STATIC_READ);
}

void GLRenderer::drawSprites() const {
    SynchronizeOn(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    _sprite.renderProgram->bind();
    _sprite.oam->bind();
    _sprite.texture->bind();
    _sprite.renderProgram->setUniform(
        "u_Texture", static_cast<int>(_sprite.texture->getTextureSlot()));
    _sprite.renderProgram->setUniformMatrix4("u_Proj",
                                             &(getProjectionMatrix())[0][0]);
    _sprite.renderProgram->setUniformMatrix4("u_Scale", &(_scale)[0][0]);
    _sprite.vertexArray.bind();
    GLCall(glDrawElements(GL_TRIANGLES,
                          _sprite.vertexArray.getIndexBuffer()->getCount(),
                          GL_UNSIGNED_INT, nullptr));
}

// ---------------- SpriteData ----------------

std::shared_ptr<GLVertexBuffer> SpriteData::createSpritesVertexBuffer(
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

SpriteData::SpriteData() {
    constexpr uint32_t kTotalNumberOfSprites = 40;

    renderProgram = std::make_unique<GLProgram>();
    renderProgram->loadShader("spriteVertex.shader", GL_VERTEX_SHADER);
    renderProgram->loadShader("fragment.shader", GL_FRAGMENT_SHADER);
    renderProgram->link();

    texture = std::make_unique<GLTexture>(nullptr, 8, 8 * 256, 1, 4, 4);

    std::shared_ptr<GLVertexBuffer> vb =
        createSpritesVertexBuffer(kTotalNumberOfSprites);

    std::shared_ptr<IndexBuffer> ib =
        generateTileSequenceIndexBuffer(kTotalNumberOfSprites);

    vertexArray.setIndexBuffer(ib);
    vertexArray.addVertexBuffer(vb);

    oam = std::make_unique<GLShaderStorageBuffer>(0);
}

// ---------------- Tile Decoder ----------------

GLTileDecoder::GLTileDecoder() {
    _tileDecoderProgram = std::make_unique<GLProgram>();
    _tileDecoderProgram->loadShader("compute.shader", GL_COMPUTE_SHADER);
    _tileDecoderProgram->link();
    _compressedTileData = std::make_unique<GLShaderStorageBuffer>(0);
}

void GLTileDecoder::decode(void* iData, size_t iSize,
                           const std::unique_ptr<Texture>& iDestTexture,
                           const ColorPalette& iPalette) const {
    _compressedTileData->uploadData(iData, iSize, GL_STATIC_READ);
    _compressedTileData->bind();

    _tileDecoderProgram->bind();
    int textureBufferRef = 0;
    _tileDecoderProgram->setUniform("u_ImageOutput", textureBufferRef);
    _tileDecoderProgram->setUniformMatrix4("u_Palette", iPalette);

    iDestTexture->associateToWritableBuffer(0);

    _tileDecoderProgram->execute(256, 1, 1);
}

// ---------------- BackgroundData ----------------

BackgroundData::BackgroundData() {
    constexpr uint32_t kTilesPerSide    = 32;
    constexpr uint32_t kBackgroundTiles = kTilesPerSide * kTilesPerSide;

    texture = std::make_unique<GLTexture>(nullptr, 8, 8 * 256, 1, 4, 4);

    std::shared_ptr<GLVertexBuffer> vb =
        createTileGridVertexBuffer(kTilesPerSide, kBackgroundTiles);

    std::shared_ptr<IndexBuffer> ib =
        generateTileSequenceIndexBuffer(kBackgroundTiles);

    vertexArray.setIndexBuffer(ib);
    vertexArray.addVertexBuffer(vb);
}

std::shared_ptr<GLVertexBuffer> BackgroundData::createTileGridVertexBuffer(
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
