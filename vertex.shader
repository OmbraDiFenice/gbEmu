#version 420 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in float a_TileId;

out vec2 v_TexCoord;

uniform mat3 u_Proj;
uniform float u_TilemapWidth;

mat4x2 square = {
{ 0.0f, 0.0f },
{ 1.0f, 0.0f },
{ 1.0f, 1.0f },
{ 0.0f, 1.0f },
};

void main()
{
    vec2 corner = square[gl_VertexID % 4];
    v_TexCoord = vec2((a_TileId + corner.x) / u_TilemapWidth, corner.y);
    gl_Position = vec4(u_Proj * a_Position, 1.0);
}
