#include "camera.h"

Camera::Camera()
{
}


void Camera::setProperties(vec3 position, vec3 lookAt, vec3 up) {
    m_position = position;
    m_lookAt = lookAt;
    m_up = up;
}

void Camera::setProjection(float fovy, float aspect, float near, float far) {
    fovy = fovy * (PI/180.f) ; // degree to radian

    float f = 1.f/tanf(fovy/2.f);

    m_projection = {-f / aspect, 0.f,    0.f,                                        0.f,
                   0.f,         f,      0.f,                                        0.f,
                   0.f,         0.f,    (far + near) / (far - near),               1.f,
                   0.f,         0.f,    (-2*far*near / (far - near)),                0.f};
}

void Camera::setWorldToView() {
//    vec3 left{1, 0, 0};
//    vec3 up{0, 1, 0};
//    vec3 forward{0, 0, 1};

//    vec3 position{0.2f, 1.f, 3.f};

//    lookAt(position, position + forward, up);

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

    m_worldToView = {s.x, u.x, f.x, 0.0f,
                     s.y, u.y, f.y, 0.0f,
                     s.z, u.z, f.z, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f};

//    m_worldToView.m_data[3][0] = eye.x;
//    m_worldToView.m_data[3][1] = eye.y;
//    m_worldToView.m_data[3][2] = eye.z;

        mat4 translation;
        translation.identity();
        translation.m_data[3][0] = -eye.x;
        translation.m_data[3][1] = -eye.y;
        translation.m_data[3][2] = -eye.z;

        m_worldToView = m_worldToView * translation;

        vec4 P = {1, 1, 1, 1};

        vec4 Pk = m_worldToView * P;

        Pk = m_projection * Pk;

        //std::cerr << "Pk: " << Pk << '\n';
}

void Camera::sendToShader(GLuint programId) const { 
//    mat4 m1;
//    m1.identity();
//    mat4 m2;
//    m2.identity();
    //m2 = m_projection * m_worldToView;


//    glUniformMatrix4fv(glGetUniformLocation(programId, "modelview"), 1, GL_FALSE, m1.data());
//    glUniformMatrix4fv(glGetUniformLocation(programId, "projection"), 1, GL_FALSE, m2.data());


    glUniformMatrix4fv(glGetUniformLocation(programId, "modelview"), 1, GL_FALSE, m_worldToView.data());
    glUniformMatrix4fv(glGetUniformLocation(programId, "projection"), 1, GL_FALSE, m_projection.data());

}
