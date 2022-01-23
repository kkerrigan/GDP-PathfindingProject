#version 420

in vec4 vertUV_x2;

out vec4 finalOutputColour;

uniform vec4 objectDiffuse;

void main() {
	
	finalOutputColour = objectDiffuse;
}