#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirectionalLight {
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

struct Spotlight {
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float cutoff;
	float outerCutoff;
	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform DirectionalLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Spotlight spotlight;
uniform vec3 viewPos;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

out vec4 fragColor;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
	// properties
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - fragPos);
	// directional lighting
	vec3 lighting = calculateDirectionalLight(dirLight, norm, viewDir);
	// point lights
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		lighting += calculatePointLight(pointLights[i], norm, fragPos, viewDir);
    // spotlight
	lighting += calculateSpotlight(spotlight, norm, fragPos, viewDir);
	// final result
	fragColor = vec4(lighting, 1.0f);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	// ambient shading
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb;
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;
	// result
	vec3 lighting = ambient + diffuse + specular;
	return lighting;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	// ambient shading
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb;
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	// result
	vec3 lighting = ambient + diffuse + specular;
	return lighting;
}

vec3 calculateSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	// ambient shading
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb;
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	// spotlight intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff)/epsilon, 0.0f, 1.0f);
	diffuse *= intensity;
	specular *= intensity;
	// result
	vec3 lighting = diffuse + specular;
	return lighting;
}