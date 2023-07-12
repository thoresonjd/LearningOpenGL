#version 330 core
in vec3 fragPos;
in vec3 normal;
out vec4 fragColor;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
void main() {
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
	vec3 lighting = ambient + diffuse + specular;
	fragColor = vec4(lighting * objectColor, 1.0f);
}