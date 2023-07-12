#version 330 core
in vec3 fragPos;
in vec3 normal;
out vec4 fragColor;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
void main() {
	// ambient lighting
	float ambientFactor = 0.1f;
	vec3 ambient = ambientFactor * lightColor;
	// diffuse lighting
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;
	// final result
	vec3 lighting = ambient + diffuse;
	fragColor = vec4(lighting * objectColor, 1.0f);
}