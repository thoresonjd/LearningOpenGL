#version 330 core
layout (location = 0) in vec3 aPos;	   // attribute position 0
layout (location = 1) in vec3 aNormal; // attribute position 1
out vec3 position;
out vec3 normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
	normal = mat3(transpose(inverse(model))) * aNormal;
	position = vec3(model * vec4(aPos, 1.0f));
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}