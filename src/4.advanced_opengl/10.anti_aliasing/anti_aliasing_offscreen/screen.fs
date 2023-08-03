#version 330 core
out vec4 fragColor;
in vec2 texCoords;
uniform sampler2D screenTexture;
void main() {
    vec3 col = texture(screenTexture, texCoords).rgb;
    float grayscale = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
    fragColor = vec4(vec3(grayscale), 1.0);
} 