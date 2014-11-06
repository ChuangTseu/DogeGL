#ifndef CAMERA_H
#define CAMERA_H

#include "MathsTools/mat4.h"
#include "MathsTools/vec3.h"
#include "MathsTools/maths.h"

#include "GL.h"

class Camera
{
    mat4 m_worldToView;

    //WARNING, No translation
    mat4 m_pureView;

    vec3 m_position;
    vec3 m_lookAt;
    vec3 m_up;

public:
    Camera();

    void setProperties(vec3 position, vec3 lookAt, vec3 up);

    void lookAt(const vec3& eye, const vec3& center, const vec3& up);

    const mat4& getView();
    const mat4 &getPureViewNoTranslation();
};

#endif // CAMERA_H
