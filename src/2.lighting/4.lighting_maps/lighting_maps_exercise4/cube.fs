#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

out vec4 fragColor;

void main() {
	// ambient lighting
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;

	// diffuse lighting
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb;

	// specular lighting
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

	// emission
	vec3 emission = texture(material.emission, texCoord).rgb;

	// final result
	vec3 lighting = ambient + diffuse + specular + emission;
	fragColor = vec4(lighting, 1.0f);
}