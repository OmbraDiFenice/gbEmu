#pragma once

#include <core/emu/screen/ColorPalette.h>
#include <core/ui/Program.h>
#include <core/ui/Renderer.h>
#include <core/ui/ShaderStorageBuffer.h>
#include <core/ui/Texture.h>
#include <core/ui/opengl/GLBuffer.h>

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

   private:
    void initBackround();
    void initOam();

    std::shared_ptr<IndexBuffer> createTileGridIndexBuffer(
        const uint32_t iTotalNumberOfTiles) const;
    std::shared_ptr<GLVertexBuffer> createTileGridVertexBuffer(
        const uint32_t iTilePerColumn,
        const uint32_t iTotalNumberOfTiles) const;

   private:
    GLVertexArray _backgroundVertexArray;
    mutable bool _backgroundDataDirty;
    std::unique_ptr<Texture> _backgroundTileTexture;

    ColorPalette _colorPalette;

    std::unique_ptr<Program> _tileDecoderProgram;
    std::unique_ptr<ShaderStorageBuffer> _compressedTileData;

    std::unique_ptr<Program> _renderProgram;
    std::unique_ptr<ShaderStorageBuffer> _rendererShaderData;

    glm::mat4 _scale;

   private:
    GLVertexArray _spriteVertexArray;
    std::unique_ptr<Program> _spriteRenderProgram;
};
