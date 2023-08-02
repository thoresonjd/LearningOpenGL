#version 330 core
in vec2 texCoords;
struct Material {
	sampler2D texture_diffuse1;
};
uniform Material material;
out vec4 fragColor;
void main() {
	fragColor = texture(material.texture_diffuse1, texCoords);
}