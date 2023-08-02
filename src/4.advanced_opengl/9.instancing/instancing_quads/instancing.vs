#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;
out vec3 fColor;
void main() {
	// vec2 pos = aPos * (gl_InstanceID / 100.0f); // scale each instance differently based on ID
	gl_Position = vec4(aPos + aOffset, 0.0f, 1.0f);
	fColor = aColor;
}