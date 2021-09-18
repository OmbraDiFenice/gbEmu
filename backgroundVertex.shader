#version 430

layout(location = 0) in vec3 a_Position;
layout(location = 1) in uint a_TileId;
layout(binding = 0) buffer shaderData {
    uint backgroundTileMap[256];
} data;

uniform mat4 u_Proj;
uniform mat4 u_Scale;
uniform int u_SignedTileIndexOffset = 128;

out vec2 v_TexCoord;

const mat4x2 k_Square = {
{ 0.0f, 0.0f },
{ 1.0f, 0.0f },
{ 1.0f, 1.0f },
{ 0.0f, 1.0f },
};
const float k_TotalTilesInTileMap = 256.0f;

int getTileIndex(uint iTileNumber) {
    uint x = 67305985u;
    uint intIndex = iTileNumber / 4u;
    uint byteInInt = iTileNumber % 4u;
    return int((data.backgroundTileMap[intIndex] >> (byteInInt * 8u)) & 0xFFu) + u_SignedTileIndexOffset;
}

void main()
{
    gl_Position = u_Proj * u_Scale * vec4(a_Position, 1.0);

    vec2 corner = k_Square[gl_VertexID % 4];
    int tileId = getTileIndex(uint(a_TileId));
    v_TexCoord = vec2(corner.x, (tileId + corner.y) / k_TotalTilesInTileMap);
}
