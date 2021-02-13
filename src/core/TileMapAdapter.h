#pragma once
#include <core/emu/Video.h>
#include <core/ui/GLTexture.h>

class TileMapAdapter {
   public:
    GLTexture toTexture(unsigned char* iTileMap);

   private:
    void verticalMirror(unsigned char* ioBuffer);

    void mapToRgb(unsigned char* iBuffer, unsigned char* oRgbBuffer);

   private:
    uint32_t _colorMap[4] = {0xF0E0D0, 0x989898, 0x686868, 0x383838};
};
