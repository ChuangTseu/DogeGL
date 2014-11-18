#include "camera.h"

Camera::Camera()
{
}

void Camera::setProperties(vec3 position, vec3 lookAt, vec3 up) {
    m_position = position;
    m_lookAt = lookAt;
    m_up = up;
}

void Camera::lookAt(const vec3& eye, const vec3& center, const vec3& up) {
//    vec3 gl_eye = {-4, 10, -4};
//    vec3 gl_center = {0, 0, 0};
//    vec3 gl_up = {0, 1, 0};

    vec3 f = normalize(center - eye);
    vec3 s = normalize(cross(normalize(up), f));
    vec3 u = normalize(cross(f, s));

//    gl_f = vnormalize(gl_center - gl_eye);
//    gl_s = cross(vnormalize(gl_up), gl_f);
//    gl_u = vnormalize(cross(gl_f, gl_s));

//    std::cerr << "f: " << f << '\n';
//    std::cerr << "s: " << s << '\n';
//    std::cerr << "u: " << u << '\n';
//    std::cerr << '\n';

    m_worldToView = mat4{s.x, u.x, f.x, 0.0f,
                     s.y, u.y, f.y, 0.0f,
                     s.z, u.z, f.z, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f};

    m_pureView = m_worldToView;

//    m_worldToView.m_data[3][0] = eye.x;
//    m_worldToView.m_data[3][1] = eye.y;
//    m_worldToView.m_data[3][2] = eye.z;

        mat4 translation;
        translation.identity();
        translation.m_data[3][0] = -eye.x;
        translation.m_data[3][1] = -eye.y;
        translation.m_data[3][2] = -eye.z;

        m_worldToView = m_worldToView * translation;
}

const mat4& Camera::getView() {
    return m_worldToView;
}

const mat4& Camera::getPureViewNoTranslation() {
    return m_pureView;
}
