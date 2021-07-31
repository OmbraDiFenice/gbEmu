#pragma once

#include <core/ui/Texture.h>

class TilePatternAdapter {
   public:
    /** Convert a tileMap buffer from a set of color-indexed pixels to an OpenGL
     * texture object
     *
     * @param iTileMap input buffer containing pixel data, 1 byte per pixel.
     * Each byte correspond to a color index (not RGB encoded)
     * @param iWidth total width of the input buffer. Will also correspond to
     * the final texture width in pixels
     * @param iHeight total height of the input buffer. Will also correspond to
     * the final texture height in pixels
     * @param iTextureSlot slot number where to store the texture on the GPU
     *
     * @return OpenGL texture representation of the tile map, where all the
     * tiles are on 1 row
     */
    std::shared_ptr<Texture> toTexture(unsigned char* iTileMap,
                                       unsigned int iWidth,
                                       unsigned int iHeight,
                                       int iTextureSlot);

    /** Set which index in the color map colors will become transparent.
     *
     * Put this to a negative number to make all colors in the color map solid.
     */
     void setTransparentColorIndex(int iTransparentColorIndex) { _transparentColorIndex = iTransparentColorIndex; }

   private:
    /** Create a new buffer mapping each color index from the input buffer to
     * the corresponding RGB value
     *
     * The mapping is done using the internal color map table.
     *
     * @param iBuffer buffer containing 1 color index (on 1 byte) for each pixel
     * @param oRgbBuffer buffer containing 1 RGB value (on 4 bytes) for each
     * pixel
     * @param iWidth width of the input buffer
     * @param iHeight height of the input buffer
     */
    void mapToRgb(const unsigned char* iBuffer, unsigned char* oRgbBuffer,
                  unsigned int iWidth, unsigned int iHeight);

   private:
    uint32_t _colorMap[4]      = {0xFFF0E0D0, 0xFF989898, 0xFF686868, 0xFF383838};
    int _transparentColorIndex = -1;
    const int _bytesPerColor   = sizeof(_colorMap[0]);
};
