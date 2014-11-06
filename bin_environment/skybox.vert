#version 400 core

layout(location = 0) in vec3 vertex;

out vec3 skyboxTexcoord;

// Uniform
uniform mat4 PureViewProjection;

void main( void )
{
    skyboxTexcoord = vertex;

    gl_Position = PureViewProjection * vec4(vertex, 1);
}
