#version 420 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in float a_TileId;

out vec2 v_TexCoord;

uniform mat3 u_Proj;
uniform float u_TilemapHeight;

// NOTE: the y coordinate in this square are swapped so that the y texture coordinate will result in a vertically flipped image **per tile**
// this way we don't need to vertically flip the texture tile by tile in the cpu
mat4x2 square = {
{ 0.0f, 1.0f },
{ 1.0f, 1.0f },
{ 1.0f, 0.0f },
{ 0.0f, 0.0f },
};

void main()
{
    vec2 corner = square[gl_VertexID % 4];
    v_TexCoord = vec2(corner.x, (a_TileId + corner.y) / u_TilemapHeight);// the texture is a 1 big vertical stripe of tiles
    gl_Position = vec4(u_Proj * a_Position, 1.0);
}
