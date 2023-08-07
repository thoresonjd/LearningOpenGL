#version 330 core
// interface block

in VS_OUT {
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
	vec4 fragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform vec3 viewPos;
uniform vec3 lightPos;

out vec4 fragColor;

float calculateShadow(vec4 fragPosLightSpace);

void main() {
	// setup
	vec3 color = texture(diffuseTexture, fs_in.texCoords).rgb;
	vec3 normal = normalize(fs_in.normal);
	vec3 lightColor = vec3(1.0f);

	// ambient lighting
	vec3 ambient = 0.15f * color;

	// diffuse lighting
	vec3 lightDir = normalize(lightPos - fs_in.fragPos);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * color;

	// specular lighting
	vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = spec * lightColor;

	// shadow
	float shadow = calculateShadow(fs_in.fragPosLightSpace);

	// final result
	vec3 lighting = (ambient + (1.0f - shadow) * (diffuse + specular)) * color;
	fragColor = vec4(lighting, 1.0f);
}

float calculateShadow(vec4 fragPosLightSpace) {
	// perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5f + 0.5f;
	// get closest depth value from light's perspective (using [0,1] range fragPosLightSpace as coords)
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	// get the depth of the current fragment from the light's perspective
	float currentDepth = projCoords.z;
	// check whether current frag pos is in shadow
	float shadow = currentDepth > closestDepth ? 1.0f : 0.0f;
	return shadow;
}