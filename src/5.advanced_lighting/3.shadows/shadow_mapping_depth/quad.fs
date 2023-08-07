#version 330 core

in vec2 texCoords;

uniform sampler2D depthMap;
uniform float nearPlane;
uniform float farPlane;

out vec4 fragColor;

float LinearizeDepth(float depth);

void main() {
	float depthValue = texture(depthMap, texCoords).r;
	fragColor = vec4(vec3(depthValue), 1.0f);
}

// required when using a perspective projection matrix
float linearizeDepth(float depth) {
    float z = depth * 2.0f - 1.0f; // Back to NDC 
    return (2.0f * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));	
}