#version 330 core

in vec2 texCoords;

uniform sampler2D image;
uniform bool horizontal;
uniform float weight[5] = float[] (0.2270270270f, 0.1945945946f, 0.1216216216f, 0.0540540541f, 0.0162162162f);

out vec4 fragColor;

void main() {
     vec2 texOffset = 1.0f / textureSize(image, 0); // gets size of single texel
     vec3 result = texture(image, texCoords).rgb * weight[0];
     if(horizontal) {
         for(int i = 1; i < 5; i++) {
            result += texture(image, texCoords + vec2(texOffset.x * i, 0.0f)).rgb * weight[i];
            result += texture(image, texCoords - vec2(texOffset.x * i, 0.0f)).rgb * weight[i];
         }
     } else {
         for(int i = 1; i < 5; i++) {
             result += texture(image, texCoords + vec2(0.0f, texOffset.y * i)).rgb * weight[i];
             result += texture(image, texCoords - vec2(0.0f, texOffset.y * i)).rgb * weight[i];
         }
     }
     fragColor = vec4(result, 1.0f);
}