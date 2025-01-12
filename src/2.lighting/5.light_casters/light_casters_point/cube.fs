#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
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

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

	// result
	vec3 result = (ambient + diffuse + specular) * attenuation;
	fragColor = vec4(result, 1.0);
}