#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

void main() {
	// use normal matrix to remove effect of wrongly scaling normal vectors when non-uniform scaling occurs
	// expensive for shaders to calculate, typically passed as a uniform
	mat3 normalMatrix = mat3(transpose(inverse(model)));
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	fragPos = vec3(model * vec4(aPos, 1.0));
	normal = normalMatrix * aNormal;
	texCoord = aTexCoord;
}