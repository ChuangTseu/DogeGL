#ifndef CAMERA_H
#define CAMERA_H

#include "MathsTools/mat4.h"
#include "MathsTools/vec3.h"

#include "GL/glew.h"

#include <math.h>

#define PI		3.14159265358979323846f
#define PI_2		1.57079632679489661923f
#define PI_4		0.78539816339744830962f

class Camera
{
    mat4 m_worldToView;
    mat4 m_projection;

    vec3 m_position;
    vec3 m_lookAt;
    vec3 m_up;

public:
    Camera();

    void setProperties(vec3 position, vec3 lookAt, vec3 up);

    void setProjection(float fovy, float aspect, float near, float far);

    void setWorldToView();

    void lookAt(const vec3& eye, const vec3& center, const vec3& up);

    void sendToShader(GLuint programId) const;
};

#endif // CAMERA_H
