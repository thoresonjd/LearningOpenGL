#version 330 core
in vec3 fragPos;
in vec3 norm;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
out vec4 fragColor;
void main() {
	// ambient component
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse component
	vec3 normal = normalize(norm);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// result
	vec3 result = (ambient + diffuse) * objectColor;
	fragColor = vec4(result, 1.0);
}