#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
out vec3 lighting;
void main() {
	// use normal matrix to remove effect of wrongly scaling normal vectors when non-uniform scaling occurs
	// expensive for shaders to calculate, typically passed as a uniform
	mat3 normalMatrix = mat3(transpose(inverse(model)));
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	vec3 fragPos = vec3(model * vec4(aPos, 1.0));
	vec3 normal = normalMatrix * aNormal;

	// ambient lighting
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;
	// diffuse lighting
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;
	// specular lighting
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	int shininess = 32;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
	vec3 specular = specularStrength * spec * lightColor;
	// final result
	lighting = ambient + diffuse + specular;
}