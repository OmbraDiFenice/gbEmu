#version 420 core

layout(location = 0) in vec3 a_Position;

out vec2 v_TexCoord;
mat4x2 square = {
{ 0.0f, 0.0f },
{ 1.0f, 0.0f },
{ 1.0f, 1.0f },
{ 0.0f, 1.0f },
};
uint tileId = 1;

void main()
{
    gl_Position = vec4(a_Position, 1.0);

    vec2 corner = square[gl_VertexID % 4];
    v_TexCoord = vec2(corner.x, (tileId + corner.y) / 256);
}
