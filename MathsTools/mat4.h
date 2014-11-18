#ifndef MAT4_H
#define MAT4_H

#include <algorithm>
#include <iostream>
#include <cassert>

#include "vec4.h"

#include "vec3.h"

#include "maths.h"

struct mat4
{
    enum { N = 4 };

    float m_data[N][N];

    //constexpr 
	float* pBegin() {
        return (float*) m_data;
    }

	float* pBegin() const {
		return (float*)m_data;
	}

    //constexpr 
	float* pEnd() {
        return (float*) m_data + N*N;
    }

	float* pEnd() const {
		return (float*)m_data + N*N;
	}


public:
    //constexpr 
	float* data() {
        return (float*) m_data;
    }

	const float* data() const {
		return (float*)m_data;
	}

    void fill(float f) {
        std::fill(pBegin(), pEnd(), f);
    }

    float* operator[] (int i) {
        return m_data[i];
    }

    void identity() {
        fill(0.f);

        for(int i = 0; i < 4; ++i) {
            m_data[i][i] = 1.f;
        }
    }

    static mat4 Identity() {
        mat4 ret;
        ret.identity();

        return ret;
    }

    static mat4 Projection(float fovy, float aspect, float near, float far) {
        fovy = fovy * (PI/180.f) ; // degree to radian

        float f = 1.f/tanf(fovy/2.f);

        return        {-f / aspect, 0.f,    0.f,                                        0.f,
                       0.f,         f,      0.f,                                        0.f,
                       0.f,         0.f,    (far + near) / (far - near),               1.f,
                       0.f,         0.f,    (-2*far*near / (far - near)),                0.f};
    }

    static mat4 LookAt(const vec3& eye, const vec3& center, const vec3& up);

    mat4& operator+=(const mat4& other) {
        std::transform(pBegin(), pEnd(), other.pBegin(), pBegin(), std::plus<float>());

        return *this;
    }

    mat4& operator-=(const mat4& other) {
        std::transform(pBegin(), pEnd(), other.pBegin(), pBegin(), std::minus<float>());

        return *this;
    }

    mat4& operator*=(const mat4& other) {
        mat4 mat_tmp = *this;

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                float sum = 0;

                for (int k = 0; k < N; ++k) {
                    sum += m_data[k][i] * other.m_data[j][k];
                }

                mat_tmp.m_data[j][i] = sum;
            }
        }

        *this = mat_tmp;

        //transpose();

        return *this;
    }

    mat4& compMult(const mat4& other) {
        std::transform(pBegin(), pEnd(), other.pBegin(), pBegin(), std::multiplies<float>());

        return *this;
    }

    void transpose() {
        for (int i = 0; i < N - 1; ++i) {
            for (int j = i + 1; j < N; ++j) {
                std::swap(m_data[i][j], m_data[j][i]);
            }
        }
    }

    mat4& rotate(const vec3& axis, float angle) {
        assert(norm(axis) == 1.f);

        angle = angle * (PI/180.f); // degree to radian
        float c = cosf(angle);
        float s = sinf(angle);
        float x = axis.x;
        float y = axis.y;
        float z = axis.z;

        mat4 r{x*x*(1 - c) + c,     y*x*(1 - c) + z*s,  x*z*(1 - c) - y*s,  0.f,
               x*y*(1 - c) - z*s,   y*y*(1 - c) + c,    y*z*(1 - c) + x*s,  0.f,
               x*z*(1 - c) + y*s,   y*z*(1 - c) - x*s,  z*z*(1 - c) + c,    0.f,
               0.f,                 0.f,                0.f,                1.f
              };

        return (*this) *= r;
    }

    mat4& translate(float x, float y, float z) {

        mat4 r{1.f,                 0.f,                0.f,                0.f,
               0.f,                 1.f,                0.f,                0.f,
               0.f,                 0.f,                1.f,                0.f,
               x,                 y,                z,                1.f
              };

        return (*this) *= r;
    }

    mat4& translate(vec3 direction) {

        return translate(direction.x, direction.y, direction.z);
    }

    mat4& scale(float x, float y, float z) {

        mat4 r{x,                 0.f,                0.f,                0.f,
               0.f,                y,                0.f,                0.f,
               0.f,                 0.f,                z,                0.f,
               0.f,                 0.f,                0.f,                1.f
              };

        return (*this) *= r;
    }

    mat4& operator/=(const mat4& other) {
        std::transform(pBegin(), pEnd(), other.pBegin(), pBegin(), std::divides<float>());

        return *this;
    }

};

inline std::ostream& operator<<(std::ostream& os, const mat4& v) {
    for (int i = 0; i < mat4::N; ++i) {
        for (const float& e : v.m_data[i]) {
            os << e << ' ';
        }

        std::cout << '\n';
    }

    return os;
}

inline mat4 matCompMult(mat4 lhs, const mat4& rhs) {
    return lhs.compMult(rhs);
}

inline mat4 operator+(mat4 lhs, const mat4& rhs) {
    return lhs += rhs;
}

inline mat4 operator-(mat4 lhs, const mat4& rhs) {
    return lhs -= rhs;
}

inline mat4 operator*(mat4 lhs, const mat4& rhs) {
    return lhs *= rhs;
}

inline mat4 operator/(mat4 lhs, const mat4& rhs) {
    return lhs /= rhs;
}

inline vec4 operator*(const mat4 lhs, const vec4& rhs) {
    vec4 ret{0.f, 0.f, 0.f, 0.f};

    for (int j = 0; j < 4; ++j) {
        for (int i = 0; i < 4; ++i) {
            ret.m_data[j] += lhs.m_data[i][j] * rhs.m_data[i];
        }
    }

    return ret;

}

inline mat4 mat4::LookAt(const vec3& eye, const vec3& center, const vec3& up) {
    vec3 f = normalize(center - eye);
    vec3 s = normalize(cross(normalize(up), f));
    vec3 u = normalize(cross(f, s));

    mat4 worldToView = mat4{s.x, u.x, f.x, 0.0f,
                        s.y, u.y, f.y, 0.0f,
                        s.z, u.z, f.z, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f};

    mat4 translation;
    translation.identity();
    translation.m_data[3][0] = -eye.x;
    translation.m_data[3][1] = -eye.y;
    translation.m_data[3][2] = -eye.z;

    return worldToView * translation;
}

#endif // MAT4_H
