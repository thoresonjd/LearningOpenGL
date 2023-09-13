#version 330 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;

struct Light {
    vec3 position;
    vec3 color;  
    float linear;
    float quadratic;
};
uniform Light light;

void main() {
    // retrieve data from gbuffer
    vec3 fragPos = texture(gPosition, texCoords).rgb;
    vec3 normal = texture(gNormal, texCoords).rgb;
    vec3 diffuse = texture(gAlbedo, texCoords).rgb;
    float ambientOcclusion = texture(ssao, texCoords).r;
    // then calculate lighting as usual
    vec3 ambient = vec3(0.3 * diffuse * ambientOcclusion);
    vec3 lighting = ambient; 
    vec3 viewDir = normalize(-fragPos); // viewpos is (0.0.0)
    // diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    diffuse = max(dot(normal, lightDir), 0.0) * diffuse * light.color;
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 8.0);
    vec3 specular = light.color * spec;
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * distance * distance);
    diffuse *= attenuation;
    specular *= attenuation;
    lighting += diffuse + specular;
    fragColor = vec4(lighting, 1.0);
}