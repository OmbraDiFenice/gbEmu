#pragma once

#include <core/graphics/Program.h>
#include <core/graphics/Renderer.h>
#include <core/graphics/ShaderStorageBuffer.h>
#include <core/graphics/Texture.h>
#include <core/graphics/opengl/GLBuffer.h>

class GLTileDecoder {
   public:
    GLTileDecoder();
    void decode(void* iData, size_t iSize,
                const std::unique_ptr<Texture>& iDestTexture,
                const ColorPalette& iPalette) const;

   private:
    std::unique_ptr<Program> _tileDecoderProgram;
    std::unique_ptr<ShaderStorageBuffer> _compressedTileData;
};

struct BackgroundData {
    BackgroundData();

    GLVertexArray vertexArray;
    std::unique_ptr<Texture> texture;
    std::unique_ptr<Program> renderProgram;
    std::unique_ptr<ShaderStorageBuffer> renderShaderData;

   private:
    std::shared_ptr<GLVertexBuffer> createTileGridVertexBuffer(
        const uint32_t iTilePerColumn,
        const uint32_t iTotalNumberOfTiles) const;

    static constexpr uint32_t kTextureSlot = 1;
};

struct SpriteData {
    SpriteData();

    GLVertexArray vertexArray;
    std::unique_ptr<Program> renderProgram;
    std::unique_ptr<Texture> texturePalette0;
    std::unique_ptr<Texture> texturePalette1;
    std::unique_ptr<ShaderStorageBuffer> oam;

   private:
    std::shared_ptr<GLVertexBuffer> createSpritesVertexBuffer(
        uint32_t iTotalSprites);

    static constexpr uint32_t kTexturePalette0Slot = 1;
    static constexpr uint32_t kTexturePalette1Slot = 2;
};

class GLRenderer : public Renderer {
   public:
    GLRenderer();

    void setBackgroundTileData(void* iData, size_t iSize) override;
    void setBackgroundTileMapData(void* iData, size_t iSize) override;
    void setSignedBackgroundTileMap(bool iSigned) const override;

    void setSpriteTileData(void* iData, size_t iSize) override;
    void setOam(void* iData, size_t iSize) override;

    void drawBackground() const override;
    void drawSprites() const override;
    void clear(float iRed, float iGreen, float iBlue,
               float iAlpha) const override;
    void flush() const override;

    void setScale(float iScale) override;
    const glm::mat4& getProjectionMatrix() const override;

    /** Note: manually call setBackgroundTileData after this to update the
     * texture
     *
     * @param iPalette
     */
    void setBackgroundPalette(const ColorPalette& iPalette) override;
    /** Note: manually call setSpriteTileData after this to update the texture
     *
     * @param iPalette
     */
    void setObjectPalette0(const ColorPalette& iPalette) override;
    /** Note: manually call setSpriteTileData after this to update the texture
     *
     * @param iPalette
     */
    void setObjectPalette1(const ColorPalette& iPalette) override;

   private:
    GLTileDecoder _tileDecoder;
    glm::mat4 _scale;

    BackgroundData _background;
    SpriteData _sprite;

    ColorPalette _colorPalette;
    ColorPalette _obj0ColorPalette;
    ColorPalette _obj1ColorPalette;
};
