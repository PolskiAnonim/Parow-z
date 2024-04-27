#version 330

layout (location =0) in vec3 position;
layout (location =1) in vec3 normal;
layout (location =2) in vec2 texCoords;


out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 cam;

void main(void) {
    TexCoords = texCoords;
    gl_Position=cam*model*  vec4(position, 1.0);
    FragPos = vec3(model *  vec4(position, 1.0));
    Normal =  normal;
}
