#pragma once

#include <core/emu/screen/Video.h>
#include <core/emu/screen/VideoItem.h>
#include <core/emu/screen/TileBuffer.h>

class BackgroundMap : VideoItem {
   public:
    BackgroundMap();
    void reindex(const unsigned char* iBackgroundTileMap, bool iSignedIndexes);

    void render(const Renderer& renderer, const Video& iVideo, Program& ioProgram) const override;

   private:
    TileBuffer backgroundMap[32][32];
};
