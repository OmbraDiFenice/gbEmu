#version 430

layout(location = 0) in uint a_TileId;
layout(binding = 0) buffer shaderData {
    uint data[256];
} oam;

uniform mat4 u_Proj;
uniform mat4 u_Scale;

out vec2 v_TexCoord;

const float kTotalSprites = 256.0f;
const mat4x2 k_TexCoordSquare = {
{ 0.0f, 0.0f },
{ 1.0f, 0.0f },
{ 1.0f, 1.0f },
{ 0.0f, 1.0f },
};
const mat4x2 k_TileCoordSquare = {
{ 0.0f, 8.0f },
{ 8.0f, 8.0f },
{ 8.0f, 0.0f },
{ 0.0f, 0.0f },
};

uint getByte(const uint iSpriteData, const uint iByteIndex) {
    return (iSpriteData >> (iByteIndex * 8u)) & 0xFFu;
}

vec4 getPosition(const uint iSpriteData) {
    return vec4(
    float(getByte(iSpriteData, 1)) + k_TileCoordSquare[gl_VertexID % 4].x - 8,
    float(getByte(iSpriteData, 0)) + k_TileCoordSquare[gl_VertexID % 4].y - 16,
    0.0f,
    1.0f);
}

uint getTileIndex(const uint iSpriteData) {
    return getByte(iSpriteData, 2);
}

void main()
{
    const uint spriteData = oam.data[a_TileId];

    gl_Position = u_Proj * u_Scale * getPosition(spriteData);
    uint tileId = getTileIndex(spriteData);

    vec2 corner = k_TexCoordSquare[gl_VertexID % 4];
    v_TexCoord = vec2(corner.x, (tileId + corner.y) / kTotalSprites);
}
