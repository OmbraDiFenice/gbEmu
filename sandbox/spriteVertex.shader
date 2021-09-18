#version 430

layout(location = 0) in vec3 a_Position;
layout(location = 1) in uint a_TileId;

uniform mat4 u_Proj;
uniform mat4 u_Scale;

out vec3 v_Color;

const float kTotalSprites = 40;

void main()
{
    gl_Position = u_Proj * u_Scale * vec4(a_Position, 1.0);
    v_Color = vec3(float(a_TileId) / kTotalSprites, 0.0f, 0.0f);
}
