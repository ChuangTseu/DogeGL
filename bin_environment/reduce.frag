#version 400 core

uniform sampler2D texSampler;

uniform int screenWidth;
uniform int screenHeight;

layout(location = 0, index = 0) out vec4 fragColor;

#define approx(x, val) (val - 0.00001 < x && x < val + 0.00001)

void main( void )
{
    float previousOffset = 1.0 / (screenWidth * 4.0);
    float previousStride = 1.0 / (screenWidth * 2.0);

    float offsetX = 1.0 / (screenWidth * 4.0);
    float offsetY = 1.0 / (screenHeight * 4.0);

    float strideX = 1.0 / (screenWidth * 2.0);
    float strideY = 1.0 / (screenHeight * 2.0);

    vec2 screenTexCoord = vec2(gl_FragCoord.x/screenWidth - offsetX, gl_FragCoord.y/screenHeight - offsetY);

//    vec2 screenTexCoord = vec2(gl_FragCoord.x*2.0/screenWidth, gl_FragCoord.y/screenHeight) + vec2(previousOffset/2, previousOffset/2);

//    fragColor = (texture(texSampler, screenTexCoord)
//            + textureOffset(texSampler, screenTexCoord, ivec2(1, 0))
//            + textureOffset(texSampler, screenTexCoord, ivec2(1, 1))
//            + textureOffset(texSampler, screenTexCoord, ivec2(0, 0)))
//            / 4;

    fragColor = (texture(texSampler, screenTexCoord)
            + texture(texSampler, screenTexCoord + vec2(strideX, 0))
            + texture(texSampler, screenTexCoord + vec2(0, strideY))
            + texture(texSampler, screenTexCoord + vec2(strideX, strideY)))
            / 4.0;

//    if (approx())


//    fragColor = vec4(screenTexCoord, 0, 0);
}
