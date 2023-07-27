#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
	vec3 color;
} gs_in[];

out vec3 fragmentColor;

void buildHouse(vec4 position);

void main() {
	buildHouse(gl_in[0].gl_Position);
}

void buildHouse(vec4 position) {
	fragmentColor = gs_in[0].color;
	gl_Position = position + vec4(-0.2f, -0.2f, 0.0f, 0.0f); // bottom left
	EmitVertex();
	gl_Position = position + vec4(0.2f, -0.2f, 0.0f, 0.0f);  // bottom right
	EmitVertex();
	gl_Position = position + vec4(-0.2f, 0.2f, 0.0f, 0.0f);  // top left
	EmitVertex();
	gl_Position = position + vec4(0.2f, 0.2f, 0.0f, 0.0f);   // top right
	EmitVertex();
	gl_Position = position + vec4(0.0f, 0.4f, 0.0f, 0.0f);   // top
	fragmentColor = vec3(1.0f, 1.0f, 1.0f);                  // snow on roof
	EmitVertex();
	EndPrimitive();
}