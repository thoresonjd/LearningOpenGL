#version 330 core
in vec2 texCoord;
out vec4 fragColor;
uniform sampler2D texture1;
uniform sampler2D texture2;
void main() {
	// linearly interpolate between both textures (80% texture1, 20% texture2)
	fragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
}