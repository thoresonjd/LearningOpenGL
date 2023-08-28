#version 330 core

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;

struct Light {
    vec3 position;
    vec3 color;
};

uniform Light lights[16];
uniform sampler2D diffuseTexture;
uniform vec3 viewPos;

out vec4 fragColor;

void main() {
    vec3 color = texture(diffuseTexture, fs_in.texCoords).rgb;
    vec3 normal = normalize(fs_in.normal);
    // ambient
    vec3 ambient = 0.0f * color;
    // lighting
    vec3 lighting = vec3(0.0f);
    for(int i = 0; i < 16; i++) {
        // diffuse
        vec3 lightDir = normalize(lights[i].position - fs_in.fragPos);
        float diff = max(dot(lightDir, normal), 0.0f);
        vec3 diffuse = lights[i].color * diff * color;      
        vec3 result = diffuse;        
        // attenuation (use quadratic as we have gamma correction)
        float distance = length(fs_in.fragPos - lights[i].position);
        result *= 1.0f / (distance * distance);
        lighting += result;
    }
    fragColor = vec4(ambient + lighting, 1.0f);
}