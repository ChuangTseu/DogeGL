#version 400 core

uniform sampler2D texSampler;

layout(location = 0, index = 0) out vec4 fragColor;

int screenWidth = 640;
int screenHeight = 480;

void main( void )
{
    vec2 screenTexCoord = vec2(gl_FragCoord.x/screenWidth, gl_FragCoord.y/screenHeight);

    fragColor = texture(texSampler, screenTexCoord);

//    fragColor = vec4(1,0,0,1);
}
