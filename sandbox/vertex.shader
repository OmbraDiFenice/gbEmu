#version 420 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_TileId;

uniform mat4 u_Proj;
uniform mat4 u_Scale;

out vec2 v_TexCoord;

const mat4x2 k_Square = {
{ 0.0f, 0.0f },
{ 1.0f, 0.0f },
{ 1.0f, 1.0f },
{ 0.0f, 1.0f },
};
const float k_TotalTilesInTileMap = 256.0f;

void main()
{
    gl_Position = u_Proj * u_Scale * vec4(a_Position, 1.0);

    vec2 corner = k_Square[gl_VertexID % 4];
    v_TexCoord = vec2(corner.x, (a_TileId + corner.y) / k_TotalTilesInTileMap);
}
