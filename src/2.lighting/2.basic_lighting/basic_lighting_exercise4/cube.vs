#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec3 color;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	
	// Gouraud shading
	vec3 pos = vec3(model * vec4(aPos, 1.0));
	mat3 normalMatrix = mat3(transpose(inverse(model))); // properly scale normals
	vec3 norm = normalize(normalMatrix * aNorm);

	// ambient component
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse component
	vec3 lightDir = normalize(lightPos - pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular component
	vec3 viewDir = normalize(viewPos - pos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float specularStrength = 0.5;
	float shininess = 32;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularStrength * spec * lightColor;

	// result
	vec3 lighting = ambient + diffuse + specular;
	color = lighting * objectColor;
}