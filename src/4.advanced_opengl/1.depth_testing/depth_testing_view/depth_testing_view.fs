#version 330 core
out vec4 fragColor;

float near = 0.1f;
float far = 100.0f;

float linearizeDepth(float depth);

void main() {
	float depth = linearizeDepth(gl_FragCoord.z) / far;
	fragColor = vec4(vec3(depth), 1.0f);
}

float linearizeDepth(float depth) {
	float ndc = depth * 2.0f - 1.0f;
	return (2.0f * near * far) / (far + near - ndc * (far - near));
}