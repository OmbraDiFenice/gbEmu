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
     * @param iTileWidth height of a single tile in the tile map, in bytes (each
     * byte correspond to 1 pixel)
     * @param iTileHeight width of a single tile in the tile map, in bytes (each
     * byte correspond to 1 pixel)
     * @param iTextureSlot slot number where to store the texture on the GPU
     *
     * @return OpenGL texture representation of the tile map, where all the
     * tiles are on 1 row
     */
    std::shared_ptr<Texture> toTexture(unsigned char* iTileMap,
                                       unsigned int iWidth,
                                       unsigned int iHeight,
                                       unsigned int iTileWidth,
                                       unsigned int iTileHeight,
                                       int iTextureSlot);

    /** Set which index in the color map colors will become transparent.
     *
     * Put this to a negative number to make all colors in the color map solid.
     */
     void setTransparentColorIndex(int iTransparentColorIndex) { _transparentColorIndex = iTransparentColorIndex; }

   private:
    /** Vertically flip the pixels in the buffer
     *
     * @param ioBuffer buffer containing the pixels to flip
     * @param iWidth width of the buffer
     * @param iHeight height of the buffer
     */
    void verticalMirror(unsigned char* ioBuffer, unsigned int iWidth,
                        unsigned int iHeight);

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

    /** Reorder pixels in the buffer so that the texture will be 1 line of
     * texture tiles
     *
     * The resulting data in the buffer will have all the first lines of each
     * tile, then all the second lines etc.
     *
     * Assuming that the first tile is the shape of an "A" and the second is the
     * shape of a "B", we're going from a layout like this:
     *
     * <pre>
     *   ...x....|..x.x...|..x.x...|.x...x..|.xxxxx..|x.....x.|x.....x.|........
     *   xxxxx...|x....x..|x....x..|xxxxxx..|x.....x.|x.....x.|xxxxxx..|........
     * </pre>
     *
     * where lines of first tile are in sequence, followed by the lines of
     * second tile and so on, to a layout like this
     *
     * <pre>
     *   ...x....|xxxxx...|
     *   ..x.x...|x....x..|
     *   ..x.x...|x....x..|
     *   .x...x..|xxxxxx..|
     *   .xxxxx..|x.....x.|
     *   x.....x.|x.....x.|
     *   x.....x.|xxxxxx..|
     *   ........|........|
     * </pre>
     *
     * And so on.
     *
     * This allows to have Y texture coord always 0 and 1, so to select which
     * tile is going to be displayed all we need is it's 0-based index and we
     * only need to update the X texture coords.
     *
     * Note that "bytes" here will correspond to "pixel" since the input buffer
     * is supposed hold uncompressed data but not yet "rgb color mapped" data.
     *
     * @param ioBuffer pixel data buffer to reorder. Each byte correspond to a
     * color index (not RGB encoded)
     * @param iWidth total width of the buffer in bytes
     * @param iHeight total height of the buffer in bytes
     * @param iTileWidth single tile width in bytes
     * @param iTileHeight single tile height in bytes
     */
    void reorderData(unsigned char* ioBuffer, unsigned int iWidth,
                     unsigned int iHeight, unsigned int iTileWidth,
                     unsigned int iTileHeight);

   private:
    uint32_t _colorMap[4]      = {0xFFF0E0D0, 0xFF989898, 0xFF686868, 0xFF383838};
    int _transparentColorIndex = -1;
    const int _bytesPerColor   = sizeof(_colorMap[0]);
};
