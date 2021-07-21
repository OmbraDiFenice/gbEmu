#pragma once

#include <core/emu/screen/GbRenderer.h>
#include <core/ui/Program.h>

class Video;

class VideoItem {
   public:
    virtual void render(const GbRenderer& renderer) const = 0;
};