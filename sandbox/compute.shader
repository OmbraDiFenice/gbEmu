#version 430

layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba8) uniform image2D img_output;

void main() {
    vec4 pixel = vec4(0.0f, 0.2f, 0.5f, 1.0f);
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);

    imageStore(img_output, pixelCoords, pixel);
}
