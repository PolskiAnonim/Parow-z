#version 330

layout (location =0) in vec3 position;
layout( location =3) in vec3 instances;



uniform mat4 model;
uniform mat4 cam;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main(void) {
    vec4 instancedPosition = vec4(position, 1.0);
    instancedPosition.xyz -= instances; // Ktoœ tu wczyta³ wszystkie modele odwrotnie :>
    
    gl_Position=cam*model* instancedPosition;
}
