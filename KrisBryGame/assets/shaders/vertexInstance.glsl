#version 420

layout(location = 0) in vec4 vPosition;
layout(location = 2) in vec4 vColour;
layout(location = 3) in mat4 instanceMatrix;

out vec4 color;

uniform mat4 projection;
uniform mat4 view;

void main() {

	gl_Position = projection * view * instanceMatrix * vPosition;
	color = vColour;
}