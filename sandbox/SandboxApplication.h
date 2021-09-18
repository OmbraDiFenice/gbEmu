#pragma once

#include <Application.h>
#include <core/emu/screen/ColorPalette.h>
#include <core/ui/Program.h>
#include <core/ui/Renderer.h>
#include <core/ui/ShaderStorageBuffer.h>
#include <core/ui/Texture.h>
#include <glm/vec3.hpp>

class SandboxApplication : public Application {
   public:
    SandboxApplication();
    ~SandboxApplication();

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
