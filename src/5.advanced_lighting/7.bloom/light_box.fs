#version 330 core

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;

uniform vec3 lightColor;

void main() {        
    fragColor = vec4(lightColor, 1.0f);
    float brightness = dot(fragColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 1.0f)
        brightColor = vec4(fragColor.rgb, 1.0f);
	else
		brightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}