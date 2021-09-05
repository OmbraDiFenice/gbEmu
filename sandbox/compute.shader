#version 430

layout(local_size_x = 1) in;
layout(rgba8) uniform image2D img_output;
layout(binding = 0) buffer cd {
    uint data[];
} compressedData;

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
    vec4 pixel = vec4(0.0f, 0.2f, 0.5f, 1.0f);
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    imageStore(img_output, pixelCoords, pixel);

    uint tileIndex = gl_WorkGroupID.x;
    for (int doubleLineIdx = 0; doubleLineIdx < 4; ++doubleLineIdx)// 4 pairs of lines in each compressed tile data
    {
        uint doubleLine = compressedData.data[tileIndex * 4 + doubleLineIdx];
        for (int lineIdx = 0; lineIdx < 2; ++lineIdx) { // 2 lines each 32 bits
            for (int n = 0; n < 8; ++n) {
                uint colorIndex = decompressNth(doubleLine, lineIdx, n);

                vec4 pixel = vec4(colorIndex * 0.25f, 0.0f, 0.0f, 1.0f);
                ivec2 tileCoords = ivec2(n, doubleLineIdx * 2 + lineIdx);

                ivec2 textureCoords = toTextureCoords(tileIndex, tileCoords);
                imageStore(img_output, textureCoords, pixel);
            }
        }
    }
}
