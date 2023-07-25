#version 330 core
in vec3 position;
in vec3 normal;
uniform vec3 cameraPosition;
uniform samplerCube skybox;
out vec4 fragColor;
void main() {
	vec3 viewDir = normalize(position - cameraPosition);
	// reflection
	vec3 reflection = reflect(viewDir, normalize(normal));
	fragColor = vec4(texture(skybox, reflection).rgb, 1.0f);
	// refraction
	// float ratio = 1.00f / 1.52f; // glass
	// vec3 refraction = refract(viewDir, normalize(normal), ratio);
	// fragColor = vec4(texture(skybox, refraction).rgb, 1.0f);
}