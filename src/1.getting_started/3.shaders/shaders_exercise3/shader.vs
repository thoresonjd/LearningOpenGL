#version 330 core
layout (location = 0) in vec3 aPos;   // attribute position 0
layout (location = 1) in vec3 aColor; // attribute position 1 
out vec3 vertexColor;
void main() {
	gl_Position = vec4(aPos.x + xOffset, aPos.y, aPos.z, 1.0f);
	vertexColor = aPos;
}

// Question: Why is the bottom-left side of our triangle black?
// Answer: The position of the bottom left side is (-0.5, -0.5, 0.0), and since
// color values do not go below 0.0, the values are clamped to 0.0.
