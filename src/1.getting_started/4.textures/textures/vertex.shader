#version 330 core
layout (location = 0) in vec3 aPos;		 // attribute position 0
layout (location = 1) in vec3 aColor;	 // attribute position 1 
layout (location = 2) in vec2 aTexCoord; // attribute position 2
out vec3 vertexColor;
out vec2 texCoord;
void main() {
	gl_Position = vec4(aPos, 1.0f);
	vertexColor = aColor;
	texCoord = aTexCoord;
}