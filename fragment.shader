#version 420 core

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec2 v_TexCoord;

uniform sampler2D u_TilePatterns;
uniform mat4 u_Palette;

void main()
{
    int index = int(texture(u_TilePatterns, v_TexCoord) * 255);
    color = u_Palette[index];
    //color = vec4(v_TexCoord, 0.0, 1.0);
}
