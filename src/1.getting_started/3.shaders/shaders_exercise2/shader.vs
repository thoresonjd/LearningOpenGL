#version 330 core
layout (location = 0) in vec3 aPos;   // attribute position 0
layout (location = 1) in vec3 aColor; // attribute position 1 
out vec3 vertexColor;
uniform float xOffset;
void main() {
	gl_Position = vec4(aPos.x + xOffset, aPos.y, aPos.z, 1.0f);
	vertexColor = aColor;
}