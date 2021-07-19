#pragma once

#include <core/emu/screen/VideoItem.h>
#include <core/emu/screen/TileBuffer.h>

class SpriteMap : public VideoItem {
   public:
    SpriteMap();

    void render(const Renderer& renderer, const Video& iVideo, Program& ioProgram) const override;

   private:
    TileBuffer spriteMap[16][16];
};