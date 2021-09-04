#pragma once

#include <Application.h>
#include <core/emu/screen/GbRenderer.h>
#include <core/emu/screen/Video.h>

class EmuApplication : public Application {
   private:
    void init() override;
    void drawScreen() override;

   private:
    std::unique_ptr<GbRenderer> _renderer;
    std::unique_ptr<Video> _video;
};
