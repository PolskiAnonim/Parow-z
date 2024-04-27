#version 330

in vec4 Color;

out vec4 pixelColor; 

uniform sampler2D texture_diffuse;

void main(void) {
	pixelColor=vec4(0.68f, 0.71f, 0.74f, 0.8f);
}	