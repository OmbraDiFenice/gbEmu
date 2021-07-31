#pragma once

#include <core/ui/Texture.h>

class TilePatternAdapter {
   public:
    /** Convert a tileMap buffer from a set of color-indexed pixels to the
     * corresponding (still color-indexed) OpenGL texture object
     *
     * @param iTileMap input buffer containing pixel data, 1 byte per pixel.
     * Each byte correspond to a color index (not RGB encoded)
     * @param iWidth total width of the input buffer. Will also correspond to
     * the final texture width in pixels
     * @param iHeight total height of the input buffer. Will also correspond to
     * the final texture height in pixels
     * @param iTextureSlot slot number where to store the texture on the GPU
     *
     * @return OpenGL texture representation of the tile map
     */
    std::shared_ptr<Texture> toTexture(unsigned char* iTileMap,
                                       unsigned int iWidth,
                                       unsigned int iHeight, int iTextureSlot);
};
