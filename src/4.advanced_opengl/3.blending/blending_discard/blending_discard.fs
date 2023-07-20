#version 330 core
in vec2 texCoords;
out vec4 fragColor;
uniform sampler2D texture1;
void main() {
	vec4 texColor = texture(texture1, texCoords);
	if (texColor.a < 0.1f)
		discard; // prevent fragment from being further processed and written to the color buffer 
	fragColor = texColor;
}