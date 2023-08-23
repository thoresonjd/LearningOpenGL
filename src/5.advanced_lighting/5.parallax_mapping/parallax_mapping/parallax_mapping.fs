#version 330 core

out vec4 fragColor;

in VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float heightScale;

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir);

void main() {
    // offset texture coordinates with parallax mapping
    vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentFragPos);
    vec2 texCoords = parallaxMapping(fs_in.texCoords, viewDir);
    
    // discard fragments sampled outside default texture coordinates range
    if(texCoords.x > 1.0f || texCoords.y > 1.0f || texCoords.x < 0.0f || texCoords.y < 0.0f)
        discard;

    // sample diffuse and normal map with new texture coordinates
    vec3 color = texture(diffuseMap, texCoords).rgb;
    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0f - 1.0f);  // this normal is in tangent space
    
    // ambient
    vec3 ambient = 0.1f * color;
    
    // diffuse
    vec3 lightDir = normalize(fs_in.tangentLightPos - fs_in.tangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0f);
    vec3 diffuse = diff * color;
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);
    vec3 specular = vec3(0.2f) * spec;
    fragColor = vec4(ambient + diffuse + specular, 1.0);
}

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir) {
    float height = texture(depthMap, texCoords).r;
    vec2 P = viewDir.xy * (height * heightScale); // / viewDir.z
    return texCoords - P;
}