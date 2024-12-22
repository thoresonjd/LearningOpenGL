#version 330 core
in vec3 fragPos;
in vec3 lightPosition;
in vec3 norm;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
out vec4 fragColor;
void main() {
	// ambient component
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse component
	vec3 normal = normalize(norm);
	vec3 lightDir = normalize(lightPosition - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular component
	vec3 viewDir = normalize(-fragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
	vec3 reflectDir = reflect(-lightDir, normal);
	float specularStrength = 0.5;
	float shininess = 32;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularStrength * spec * lightColor;

	// result
	vec3 result = (ambient + diffuse + specular) * objectColor;
	fragColor = vec4(result, 1.0);
}