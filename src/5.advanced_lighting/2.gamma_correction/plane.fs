#version 330 core
in VS_OUT {
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
} fs_in;
uniform bool gammaCorrection;
uniform sampler2D floorTexture;
uniform vec3 viewPos;
uniform vec3 lightPos;
out vec4 fragColor;
void main() {
	vec3 color = texture(floorTexture, fs_in.texCoords).rgb;

	// ambient lighting
	vec3 ambient = 0.05f * color;

	// diffuse lighting
	vec3 normal = normalize(fs_in.normal);
	vec3 lightDir = normalize(lightPos - fs_in.fragPos);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * color;

	// specular lighting
	vec3 viewDir = normalize(viewPos - fs_in.fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0f;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = vec3(0.3f) * spec;

	// simple attenuation
    float distance = length(lightPos - fs_in.fragPos);
    float attenuation = 1.0 / (gammaCorrection ? distance * distance : distance);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

	if (gammaCorrection) {
		float gamma = 2.2f;
		color = pow(color, vec3(1/gamma));
	}

	// final result
	vec3 lighting = ambient + diffuse + specular;
	fragColor = vec4(lighting, 1.0f);
}