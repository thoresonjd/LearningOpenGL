#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;

uniform mat4 view;
uniform mat4 projection;

out vec2 texCoords;

void main() {
	gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0f);
	texCoords = aTexCoords;
}