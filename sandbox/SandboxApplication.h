#pragma once

#include <Application.h>
#include <core/graphics/ColorPalette.h>
#include <core/graphics/Program.h>
#include <core/graphics/Renderer.h>
#include <core/graphics/ShaderStorageBuffer.h>
#include <core/graphics/Texture.h>
#include <glm/vec3.hpp>

class SandboxApplication : public Application {
   public:
    SandboxApplication();
    ~SandboxApplication() override;

   private:
    void init() override;
    void update() override;
    void drawScreen() override;

   private:
    std::unique_ptr<Renderer> _renderer;

    unsigned char* tileMapPatterns;
    unsigned char* tileMap;
    unsigned char* spritePatterns;
    unsigned char* oam;
};
