#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 view;
uniform mat4 projection;
out vec3 texCoords;
void main() {
	texCoords = aPos;
	vec4 pos = projection * view * vec4(aPos, 1.0f);
	// force skybox to have a maximum depth value of 1.0 via perspective division of z-value
	// such that it fails the depth test when another object is rendered in front of it
	gl_Position = pos.xyww;
}