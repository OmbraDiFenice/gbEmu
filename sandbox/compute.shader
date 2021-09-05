#version 430

layout(local_size_x = 4) in;
layout(rgba8) uniform image2D u_ImageOutput;
layout(binding = 0) buffer cd {
    uint data[];
} compressedData;
uniform mat4 u_Palette;

// lineIdx should be 0 or 1
// n should be in [0, 8)
uint decompressNth(uint compressedDoubleLine, uint lineIdx, uint n) {
    uint line = (compressedDoubleLine >> (lineIdx * 16)) & uint(0xFFFF);
    uint msbLine = (line >> 8) & uint(0xFF);
    uint lsbLine = line & uint(0xFF);

    uint msb = (msbLine >> n) & uint(1);
    uint lsb = (lsbLine >> n) & uint(1);

    return (msb << 1) | lsb;
}

uint flip(uint coord) {
    return 7 - coord;
}

ivec2 toTextureCoords(uint tileIndex, ivec2 tileCoords) {
    return ivec2(flip(tileCoords.x), tileIndex * 8 + flip(tileCoords.y));
}

void main() {
    uint tileIndex = gl_WorkGroupID.x;
    uint doubleLineIdx = gl_LocalInvocationID.x;

    uint doubleLine = compressedData.data[tileIndex * 4 + doubleLineIdx];
    for (int lineIdx = 0; lineIdx < 2; ++lineIdx) { // 2 lines each 32 bits
        for (int n = 0; n < 8; ++n) {
            uint colorIndex = decompressNth(doubleLine, lineIdx, n);
            vec4 pixel = u_Palette[colorIndex];

            ivec2 tileCoords = ivec2(n, doubleLineIdx * 2 + lineIdx);
            ivec2 textureCoords = toTextureCoords(tileIndex, tileCoords);

            imageStore(u_ImageOutput, textureCoords, pixel);
        }
    }
}
