#version 420 core

layout(location = 0) out vec4 color;

in vec3 v_Position;
uniform sampler2D u_Texture;

void main()
{
    color = vec4(v_Position, 1.0);//texture(u_Texture, v_TexCoord);
}
