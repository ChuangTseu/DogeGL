#ifndef VERTEX_H
#define VERTEX_H

#include "MathsTools/vec.h"

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texcoord;
    vec3 tangent;
};

#endif // VERTEX_H
