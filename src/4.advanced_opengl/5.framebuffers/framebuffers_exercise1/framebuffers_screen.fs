#version 330 core

in vec2 texCoords;

uniform sampler2D screenTexture;

out vec4 fragColor;

void main() {
	// render fragment as normal
	fragColor = texture(screenTexture, texCoords);

	// invert colors
	// fragColor = vec4(vec3(1.0f - texture(screenTexture, texCoords)), 1.0f);

	// grayscale
	// fragColor = texture(screenTexture, texCoords);
	// float average = (fragColor.r, + fragColor.g + fragColor.b) / 3.0f;
	// fragColor = vec4(average, average, average, 1.0f);

	// grayscale with weighted channels based on human eye color sensitivity
	// fragColor = texture(screenTexture, texCoords);
	// float average = 0.2126f * fragColor.r + 0.7152 * fragColor.g + 0.0722 * fragColor.b;
	// fragColor = vec4(average, average, average, 1.0f);

	// using a kernel (convolution matrix) to sharpen the fragment
	// const float offset = 1.0f / 300.0f;  
    // vec2 offsets[9] = vec2[](
    //     vec2(-offset,  offset), // top-left
    //     vec2( 0.0f,    offset), // top-center
    //     vec2( offset,  offset), // top-right
    //     vec2(-offset,  0.0f),   // center-left
    //     vec2( 0.0f,    0.0f),   // center-center
    //     vec2( offset,  0.0f),   // center-right
    //     vec2(-offset, -offset), // bottom-left
    //     vec2( 0.0f,   -offset), // bottom-center
    //     vec2( offset, -offset)  // bottom-right    
    // );
    // float kernel[9] = float[](
    //     -1, -1, -1,
    //     -1,  9, -1,
    //     -1, -1, -1
    // );
    // vec3 sampleTex[9];
    // for(int i = 0; i < 9; i++) {
    //     sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
    // }
    // vec3 col = vec3(0.0);
    // for(int i = 0; i < 9; i++)
    //     col += sampleTex[i] * kernel[i];
    // fragColor = vec4(col, 1.0);

    // using a kernel to create a blur effect
    // const float offset = 1.0f / 300.0f;  
    // vec2 offsets[9] = vec2[](
    //     vec2(-offset,  offset), // top-left
    //     vec2( 0.0f,    offset), // top-center
    //     vec2( offset,  offset), // top-right
    //     vec2(-offset,  0.0f),   // center-left
    //     vec2( 0.0f,    0.0f),   // center-center
    //     vec2( offset,  0.0f),   // center-right
    //     vec2(-offset, -offset), // bottom-left
    //     vec2( 0.0f,   -offset), // bottom-center
    //     vec2( offset, -offset)  // bottom-right    
    // );
    // float kernel[9] = float[](
    //     1.0 / 16, 2.0 / 16, 1.0 / 16,
    //     2.0 / 16, 4.0 / 16, 2.0 / 16,
    //     1.0 / 16, 2.0 / 16, 1.0 / 16  
    // );
    // vec3 sampleTex[9];
    // for(int i = 0; i < 9; i++) {
    //     sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
    // }
    // vec3 col = vec3(0.0);
    // for(int i = 0; i < 9; i++)
    //     col += sampleTex[i] * kernel[i];
    // fragColor = vec4(col, 1.0);

    // using a kernel to create an edge detection effect
    // const float offset = 1.0f / 300.0f;  
    // vec2 offsets[9] = vec2[](
    //     vec2(-offset,  offset), // top-left
    //     vec2( 0.0f,    offset), // top-center
    //     vec2( offset,  offset), // top-right
    //     vec2(-offset,  0.0f),   // center-left
    //     vec2( 0.0f,    0.0f),   // center-center
    //     vec2( offset,  0.0f),   // center-right
    //     vec2(-offset, -offset), // bottom-left
    //     vec2( 0.0f,   -offset), // bottom-center
    //     vec2( offset, -offset)  // bottom-right    
    // );
    // float kernel[9] = float[](
    //     1, 1, 1,
    //     1, -8, 1,
    //     1, 1, 1
    // );
    // vec3 sampleTex[9];
    // for(int i = 0; i < 9; i++) {
    //     sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
    // }
    // vec3 col = vec3(0.0);
    // for(int i = 0; i < 9; i++)
    //     col += sampleTex[i] * kernel[i];
    // fragColor = vec4(col, 1.0);
}