#version 330

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 color;

out vec3 outColor;

// Uniform

uniform mat4 projection;
uniform mat4 modelview;

void main( void )
{
    outColor = color;
    gl_Position = projection * modelview * vec4(vertex, 1);
}
