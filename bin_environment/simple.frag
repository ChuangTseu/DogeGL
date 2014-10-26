#version 330

in vec3 color;

layout(location = 0, index = 0) out vec4 fragColor;

void main( void )
{
    fragColor = vec4( color, 1.0 );
    //fragColor = vec4( 1, 0, 0, 1.0 );
}
