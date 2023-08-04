#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoords;
// interface block declaration
out VS_OUT {
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
} vs_out;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
	vs_out.fragPos = aPos;
	vs_out.normal = aNorm;
	vs_out.texCoords = aTexCoords;
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}