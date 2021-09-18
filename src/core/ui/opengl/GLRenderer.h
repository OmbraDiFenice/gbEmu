#pragma once

#include <core/emu/screen/ColorPalette.h>
#include <core/ui/Program.h>
#include <core/ui/Renderer.h>
#include <core/ui/ShaderStorageBuffer.h>
#include <core/ui/Texture.h>
#include <core/ui/opengl/GLBuffer.h>

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
};

class GLRenderer : public Renderer {
   public:
    GLRenderer();

    void setBackgroundTileData(void* iData, size_t iSize) override;
    void setBackgroundTileMapData(void* iData, size_t iSize) override;
    void drawBackground() const override;
    void clear(float iRed, float iGreen, float iBlue,
               float iAlpha) const override;
    void flush() const override;
    const glm::mat4& getProjectionMatrix() const override;
    void setScale(float iScale) override;
    void setSignedBackgroundTileMap(bool iSigned) const override;

    void drawSprites() const override;
    void setSpriteTileData(void* iData, size_t iSize) override;
    void setOam(void* iData, size_t iSize) override;

   private:
    void initOam();

    std::shared_ptr<GLVertexBuffer> createSpritesVertexBuffer(
        uint32_t iTotalSprites);

   private:
    GLTileDecoder _tileDecoder;
    glm::mat4 _scale;

    BackgroundData _background;

    ColorPalette _colorPalette;
    ColorPalette _obj0ColorPalette;
    ColorPalette _obj1ColorPalette;

   private:
    GLVertexArray _spriteVertexArray;
    std::unique_ptr<Program> _spriteRenderProgram;
    std::unique_ptr<Texture> _spriteTileTexture;
    std::unique_ptr<ShaderStorageBuffer> _oam;
};
