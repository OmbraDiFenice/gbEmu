#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec3 v_Position;
out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
    v_Position = a_Position;
    gl_Position = vec4(a_Position, 1.0);
}