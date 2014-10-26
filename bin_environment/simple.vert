#version 330

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

out vec3 color;

// Uniform

uniform mat4 projection;
uniform mat4 modelview;

void main( void )
{
    gl_Position = projection * modelview * vec4(vertex, 1);

    color = normalize(normal);
}
