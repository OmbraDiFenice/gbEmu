#pragma once

#include <core/ui/Renderer.h>
#include <core/ui/Program.h>

class VideoItem {
   public:
    virtual void render(const Renderer& renderer, const Video& iVideo, Program& ioProgram) const = 0;
};