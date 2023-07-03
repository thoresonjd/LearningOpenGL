#version 330 core
in vec3 vertexColor;
in vec2 texCoord;
out vec4 fragColor;
uniform sampler2D texture;
void main() {
	fragColor = texture(texture, texCoord) * vec4(vertexColor, 1.0f);
}