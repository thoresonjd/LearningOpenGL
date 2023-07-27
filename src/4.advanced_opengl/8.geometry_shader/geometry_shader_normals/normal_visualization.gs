#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.4f;

uniform mat4 projection;

void generateLine(int index);

void main() {
	generateLine(0); // first vertex normal
	generateLine(1); // second vertex normal
	generateLine(2); // third vertex normal
}

void generateLine(int index) {
	gl_Position = projection * gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0f) * MAGNITUDE);
	EmitVertex();
	EndPrimitive();
}