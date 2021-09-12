#pragma once

#include <Application.h>
#include <core/ui/Renderer.h>

class EmuApplication : public Application {
   private:
    void init() override;
    void drawScreen() override;

   private:
    std::unique_ptr<Renderer> _renderer;
};
