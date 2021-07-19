#version 420 core

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec2 v_TexCoord;

uniform sampler2D u_TileTexture;

void main()
{
    color = texture(u_TileTexture, v_TexCoord);
    //color = vec4(v_Position + 0.5, 1.0);
}
