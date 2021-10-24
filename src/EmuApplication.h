#pragma once

#include <Application.h>
#include <core/graphics/Renderer.h>

class EmuApplication : public Application {
   public:
    EmuApplication() = default;
    ~EmuApplication() override;

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
