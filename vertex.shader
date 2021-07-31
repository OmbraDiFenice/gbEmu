#version 420 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in float a_TileId;

out vec2 v_TexCoord;

uniform mat4 u_Proj;
uniform mat4 u_Transform;
uniform float u_TilemapHeight;

mat4x2 square = {
{ 0.0f, 0.0f },
{ 1.0f, 0.0f },
{ 1.0f, 1.0f },
{ 0.0f, 1.0f },
};

void main()
{
    vec2 corner = square[gl_VertexID % 4];
    v_TexCoord = vec2(corner.x, (a_TileId + corner.y) / u_TilemapHeight);// the texture is a 1 big vertical stripe of tiles
    gl_Position = u_Proj * u_Transform * vec4(a_Position, 1.0);
}
