#ifndef LIGHT_H
#define LIGHT_H

#include "GL.h"
#include "MathsTools/vec3.h"

struct DirLight {
    vec3 m_color;
    vec3 m_direction;
};

struct PointLight {
    vec3 m_color;
    vec3 m_position;
};

class Light
{
public:
    Light();
};

#endif // LIGHT_H
