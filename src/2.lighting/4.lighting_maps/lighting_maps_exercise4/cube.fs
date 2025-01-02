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
in vec3 norm;
in vec2 texCoord;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

out vec4 fragColor;

void main() {
	// ambient component
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;

	// diffuse component
	vec3 normal = normalize(norm);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb;

	// specular component
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

	// emission component
	// essentially just adding an additional lighting component, which is just the sampled texture
	vec3 emission = texture(material.emission, texCoord).rgb;

	// result
	vec3 result = ambient + diffuse + specular + emission;
	fragColor = vec4(result, 1.0);
}