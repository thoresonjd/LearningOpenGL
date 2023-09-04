#version 330 core

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;

struct Light {
    vec3 position;
    vec3 color;
};

uniform Light lights[4];
uniform sampler2D diffuseTexture;
uniform vec3 viewPos;

void main() {
    vec3 color = texture(diffuseTexture, fs_in.texCoords).rgb;
    vec3 normal = normalize(fs_in.normal);
    // ambient
    vec3 ambient = 0.0f * color;
    // lighting
    vec3 lighting = vec3(0.0f);
    vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    for(int i = 0; i < 4; i++) {
        // diffuse
        vec3 lightDir = normalize(lights[i].position - fs_in.fragPos);
        float diff = max(dot(lightDir, normal), 0.0f);
        vec3 result = lights[i].color * diff * color;      
        // attenuation (use quadratic as we have gamma correction)
        float distance = length(fs_in.fragPos - lights[i].position);
        result *= 1.0f / (distance * distance);
        lighting += result;
                
    }
    vec3 result = ambient + lighting;
    // check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(result, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 1.0f)
        brightColor = vec4(result, 1.0f);
    else
        brightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    fragColor = vec4(result, 1.0f);
}