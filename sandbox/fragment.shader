#version 420 core

layout(location = 0) out vec4 color;

in vec3 v_Position;
uniform sampler2D u_Texture;

void main()
{
    color = texture(u_Texture, v_Position.xy);
}
