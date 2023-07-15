#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 direction;
	float cutoff;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

out vec4 fragColor;

void main() {
	// light direction
	vec3 lightDir = normalize(light.position - fragPos);

	// ambient lighting
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;

	// calculate lighting for fragments within the spotlight
	vec3 lighting;
	float theta = dot(lightDir, normalize(-light.direction));
	if (theta > light.cutoff) {
		// diffuse lighting
		vec3 norm = normalize(normal);
		float diff = max(dot(norm, lightDir), 0.0f);
		vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb;

		// specular lighting
		float specularStrength = 0.5f;
		vec3 viewDir = normalize(viewPos - fragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

		// attenuation
		float distance = length(light.position - fragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		// final result
		lighting = ambient + diffuse + specular;
	} else {
		lighting = ambient;
	}
	
	fragColor = vec4(lighting, 1.0f);
}