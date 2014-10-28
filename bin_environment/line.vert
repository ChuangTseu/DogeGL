#version 400 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 color;

out vec3 outColor;

// Uniform

uniform mat4 MVP;

void main( void )
{
    outColor = color;
    gl_Position = MVP * vec4(vertex, 1);
}
