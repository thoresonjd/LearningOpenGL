#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutoff;
	float outerCutoff;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

#define NUM_POINT_LIGHTS 4

in vec3 fragPos;
in vec3 norm;
in vec2 texCoord;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLight;

out vec4 fragColor;

vec3 calculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
	vec3 normal = normalize(norm);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 lighting = vec3(0.0);
	lighting += calculateDirectionalLight(dirLight, normal, viewDir);
	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
		lighting += calculatePointLight(pointLights[i], normal, fragPos, viewDir);
	lighting += calculateSpotLight(spotLight, normal, fragPos, viewDir);
	fragColor = vec4(lighting, 1.0);
}

vec3 calculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir) {
	// ambient component
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;

	// diffuse component
	vec3 lightDir = normalize(-light.direction); // make direction point to the light source
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb;

	// specular component
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

	// result
	return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	// ambient component
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;

	// diffuse component
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb;

	// specular component
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

	// result
	return (ambient + diffuse + specular) * attenuation;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	// ambient component
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;

	// diffuse component
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb;

	// specular component
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// spotlight (soft edges)
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;

	// result
	return ambient + diffuse + specular;
}