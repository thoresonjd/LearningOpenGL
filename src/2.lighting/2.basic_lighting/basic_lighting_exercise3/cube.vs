#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
out vec3 fragPos;
out vec3 lightPosition;
out vec3 norm;
void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	fragPos = vec3(view * model * vec4(aPos, 1.0));
	lightPosition = vec3(view * vec4(lightPos, 1.0));

	// use normal matrix to remove effect of wrongly scaling normal vectors when non-uniform scaling occurs
	// expensive for shaders to calculate, typically passed as a uniform
	mat3 normalMatrix = mat3(transpose(inverse(view * model)));
	norm = normalMatrix * aNorm;
}