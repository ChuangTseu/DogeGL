#ifndef VEC4_H
#define VEC4_H

#include <algorithm>
#include <iostream>
#include <functional>

struct vec4
{
    enum { N = 4 };

    union {
        float m_data[N];

        struct {
            float x;
            float y;
            float z;
            float w;
        };
    };


public:
    vec4& operator+=(const vec4& other) {
        std::transform(m_data, m_data + N, other.m_data, m_data, std::plus<float>());

        return *this;
    }

    vec4& operator-=(const vec4& other) {
        std::transform(m_data, m_data + N, other.m_data, m_data, std::minus<float>());

        return *this;
    }

    vec4& operator*=(const vec4& other) {
        std::transform(m_data, m_data + N, other.m_data, m_data, std::multiplies<float>());

        return *this;
    }

    vec4& operator/=(const vec4& other) {
        std::transform(m_data, m_data + N, other.m_data, m_data, std::divides<float>());

        return *this;
    }

    vec4& operator+=(float scalar) {
        for (float& e : m_data) { e += scalar; }
        return *this;
    }

    vec4& operator-=(float scalar) {
        for (float& e : m_data) { e -= scalar; }
        return *this;
    }

    vec4& operator*=(float scalar) {
        for (float& e : m_data) { e *= scalar; }
        return *this;
    }

    vec4& operator/=(float scalar) {
        for (float& e : m_data) { e /= scalar; }
        return *this;
    }

    float norm() const {
        return sqrtf(x*x + y*y + z*z + w*w);
    }

    void normalize() {
        operator/=(norm());
    }

};

inline float dot(const vec4& lhs, const vec4& rhs) {
    return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z + lhs.w*rhs.w;
}

inline float norm(const vec4& v) {
    return v.norm();
}

inline std::ostream& operator<<(std::ostream& os, const vec4& v) {
    for (const float& e : v.m_data) {
        os << e << ' ';
    }

    return os;
}

inline vec4 operator+(vec4 lhs, const vec4& rhs) {
    return lhs += rhs;
}

inline vec4 operator-(vec4 lhs, const vec4& rhs) {
    return lhs -= rhs;
}

inline vec4 operator*(vec4 lhs, const vec4& rhs) {
    return lhs *= rhs;
}

inline vec4 operator/(vec4 lhs, const vec4& rhs) {
    return lhs /= rhs;
}

#endif // VEC4_H
