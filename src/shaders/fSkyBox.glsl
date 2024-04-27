#version 330

in vec3 TexCoords;

out vec4 pixelColor; 

uniform samplerCube textureSampler;

void main(void) {
	pixelColor = texture(textureSampler, TexCoords);
}	