#version 330

layout (location =0) in vec3 position;
layout (location =1) in vec3 normal;
layout (location =2) in vec2 texCoords;
layout( location =3) in vec3 instances;


out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec3 Instance;

uniform mat4 model;
uniform mat4 cam;

void main(void) {
    TexCoords = texCoords;
    vec4 instancedPosition = vec4(position, 1.0);
    instancedPosition.xyz -= instances; 
    Instance = instancedPosition.xyz;
    Instance.y += 200; 
    
    gl_Position=cam*model* instancedPosition;
    FragPos = vec3(model * instancedPosition);
    Normal = mat3(transpose(inverse(model))) * normal;
}
