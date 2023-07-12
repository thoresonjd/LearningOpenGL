#version 330 core
in vec3 lighting;
uniform vec3 objectColor;
out vec4 fragColor;
void main() {
	fragColor = vec4(lighting * objectColor, 1.0f);
}