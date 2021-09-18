#version 420 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
flat in uint v_TextureIndex;

uniform sampler2D u_Texture[2];

void main()
{
    color = texture(u_Texture[v_TextureIndex], v_TexCoord);
}
