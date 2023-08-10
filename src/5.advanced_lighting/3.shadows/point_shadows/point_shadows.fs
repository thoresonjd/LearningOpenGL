#version 330 core

// interface block
in VS_OUT {
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform float farPlane;
uniform bool shadows;

out vec4 fragColor;

float calculateShadow(vec3 fragPos);

void main() {
	// setup
	vec3 color = texture(diffuseTexture, fs_in.texCoords).rgb;
	vec3 normal = normalize(fs_in.normal);
	vec3 lightColor = vec3(0.3f);

	// ambient lighting
	vec3 ambient = 0.3f * color;

	// diffuse lighting
	vec3 lightDir = normalize(lightPos - fs_in.fragPos);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * color;

	// specular lighting
	vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), 64.0f);
	vec3 specular = spec * lightColor;

	// shadow
	float shadow = shadows ? calculateShadow(fs_in.fragPos) : 0.0f;

	// final result
	vec3 lighting = (ambient + (1.0f - shadow) * (diffuse + specular)) * color;
	fragColor = vec4(lighting, 1.0f);
}

float calculateShadow(vec3 fragPos) {
	// get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // ise the fragment to light vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= farPlane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // test for shadows
    float bias = 0.05f; // we use a much larger bias since depth is now in [nearPlane, farPlane] range
    float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;        
    // display closestDepth as debug (to visualize depth cubemap)
    //fragColor = vec4(vec3(closestDepth / farPlane), 1.0);    
    return shadow;
}