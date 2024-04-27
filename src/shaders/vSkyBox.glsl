#version 330
layout (location =0) in vec3 position;

out vec3 TexCoords;

uniform mat4 model;
uniform mat4 cam;

void main(void) {
    TexCoords = position;
    vec4 position = cam * model *vec4(position, 1.0);
    gl_Position= position.xyww;
}
