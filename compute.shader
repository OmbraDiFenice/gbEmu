#version 430
/** Turn compressed tile data into a texture

 This compute shader is supposed to be called with the following space:
  - Workgroup: 1-dimentional where x is the number of tiles in the compressed tile data array
  - Local invocations: 1-dimentional where x is the result of bytes_per_tile / 4

 Each invocation of this shader will decode 4 bytes of compressed data, putting them at the right
 texture coordinates.
 This is the reason why the local invocation x space is fixed at 4: in the gameboy each compressed tile
 is made up of 16 bytes (each 2 bytes containing data for 2 of the 8 rows of the final decoded tile);
 since this shader can decode 4 bytes per invocation, we need 16/4 invocations to fully decode 1 tile.

 The resulting texture will be in RGBA8 format and addressable like a 1-dimensional array, where each
 element of the array will contain 1 decoded tile.

 Decompressing the data will yield one integer between 0 and 3 for each pixel of the tile. This integer
 is then used to address the color palette, which contains the mapping to the actual RGBA color.

 Input:
  compressedData - shader storage buffer seen as array of uint containing the compressed tile data
  u_Palette - 4x4 matrix where each row contains one of the 4 possible colors

 Output:
  u_ImageOutput - image2D linked with the texture. The final texture is addressable like an array, where each item contains a tile
*/
layout(local_size_x = 4) in;
layout(rgba8) uniform image2D u_ImageOutput;
layout(binding = 0) buffer cd {
    uint data[];
} compressedData;
uniform mat4 u_Palette;

/** Decompress 1 pixel at the given line from compressed data.

 This function extracts the data at position n and line lineIdx of the compressed data.
 Note that n is NOT the position of the pxiel in the tile, but the position of the bits in the compressed data
 that you want to decode. So n = 0 is asking for the LSB of the compressed data, which is the pixel at column 8
 in the tile.
 Conversely, lineIdx = 0 is the first row (top to bottom) in the tile, but be aware of 2 points:
  1. the compressed data processed by each invocation of this shader can be any of the 4 "slices" of each tile,
     so lineIdx = 0 is the first row wrt the specific "slice" of compressed data you're working on
  2. lineIdx = 0 will effectively address the rightmost half of the compressed data, which contains the data for the
     second line (top to bottom). Therefore the resulting uncompressed pixel position will have to be flipped vertically
     to be rendered correctly.

 As a result the "coordinates" given in input here are to be considered with respect to the memory layout of the
 data, and these "coordinates" happen to be the exact xy mirror of the final tile coordinates of the decoded pixel
 data given in output.

 Input:
  - compressedLines - 1 uint of compressed data containing information about 2 lines, of 8 pixel each, of the final uncompressed tile
  - lineIdx - must be 0 or 1, correspond to the index of one of the 2 "tile" lines to decode (0 is the top one)
  - n - must be in [0, 8) and correspond to the number of the bit in the row to decode

 Output:
  - a number in [0, 3], corresponding to the data held by the given memory location
*/
uint decompressNth(uint compressedLines, uint lineIdx, uint n)
{
    uint line = (compressedLines >> (lineIdx * 16)) & uint(0xFFFF);
    uint msbLine = (line >> 8) & uint(0xFF);
    uint lsbLine = line & uint(0xFF);

    uint msb = (msbLine >> n) & uint(1);
    uint lsb = (lsbLine >> n) & uint(1);

    return (msb << 1) | lsb;
}

/** Mirror a tile coordinate

 Since a tile is 8x8 pixels mirroring it along any axis just means taking the complement to 8.

 Input:
  - coord - x or y tile coordinate

 Output:
  - mirrored tile coordinate
*/
uint flip(uint coord)
{
    return 7 - coord;
}

/** Convert tile coordinates into coordinates in the final texture

 This also takes care of mirroring the pixels both horizontally and vertically.

 Input:
  - tileIndex - index of the tile we're working on
  - tileCoords - coordinates of a pixel relative to the tile as they come out from the data memory layout (i.e. x=0,y=0 is at the bottom right)

 Output:
  - coordinates of the given point relative to the whole final texture
*/
ivec2 toTextureCoords(uint tileIndex, ivec2 tileCoords)
{
    return ivec2(flip(tileCoords.x), tileIndex * 8 + flip(tileCoords.y));
}

void main() {
    uint tileIndex = gl_WorkGroupID.x;// each work group operates on 1 tile
    uint dataOffset = gl_LocalInvocationID.x;// each local invocation operates on 1 uint (32 bits), containing data worth 2 pixel lines of the final tile

    uint compressedLines = compressedData.data[tileIndex * 4 + dataOffset];

    for (int lineIdx = 0; lineIdx < 2; ++lineIdx)// loop over lines in compressedLines
    {
        for (int n = 0; n < 8; ++n)// loop over bits in a single line
        {
            uint colorIndex = decompressNth(compressedLines, lineIdx, n);// extract color index from compressed data
            vec4 pixel = u_Palette[colorIndex];// map the color index to an actual RGBA color from the palette

            ivec2 tileCoords = ivec2(n, dataOffset * 2 + lineIdx);// build the coordinates in the tile with (0,0) at bottom-right
            ivec2 textureCoords = toTextureCoords(tileIndex, tileCoords);// convert the tile coordinate into texture coordinate (mirroring on xy axis)

            imageStore(u_ImageOutput, textureCoords, pixel);// store the pixel color in the final texture
        }
    }
}
