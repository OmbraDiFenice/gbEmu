#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat3 u_Proj;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = vec4(u_Proj * a_Position, 1.0);
}
