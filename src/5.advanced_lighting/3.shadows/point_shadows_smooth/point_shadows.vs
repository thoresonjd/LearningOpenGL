#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool reverseNormals;

// interface block
out VS_OUT {
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
} vs_out;

void main() {
	vs_out.fragPos = vec3(model * vec4(aPos, 1.0f));
	// use normal matrix to remove effect of wrongly scaling normal vectors when non-uniform scaling occurs
	// expensive for shaders to calculate, typically passed as a uniform
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	if (reverseNormals)
		vs_out.normal = normalMatrix * (-1.0f * aNorm);
	else
		vs_out.normal = normalMatrix * aNorm;
	vs_out.texCoords = aTexCoords;
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}