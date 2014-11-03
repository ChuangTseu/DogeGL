#version 400 core

in vec3 outColor;

uniform vec3 userColor;
uniform bool overrideColor;

layout(location = 0, index = 0) out vec4 fragColor;

void main( void )
{
    fragColor = vec4(1,0,0, 1.0);
    if (overrideColor) {
        fragColor = vec4(userColor, 1.0);
    }
    else {
        fragColor = vec4( outColor, 1.0 );
    }
}
