#ifndef vec3_H
#define vec3_H

#include <algorithm>
#include <iostream>
#include <functional>


struct vec3
{
    union {
        float m_data[3];

        struct {
            float x;
            float y;
            float z;
        };
    };


public:
    vec3& operator+=(const vec3& other) {
        std::transform(m_data, m_data + 3, other.m_data, m_data, std::plus<float>());

        return *this;
    }

    vec3& operator-=(const vec3& other) {
        std::transform(m_data, m_data + 3, other.m_data, m_data, std::minus<float>());

        return *this;
    }

    vec3& operator*=(const vec3& other) {
        std::transform(m_data, m_data + 3, other.m_data, m_data, std::multiplies<float>());

        return *this;
    }

    vec3& operator/=(const vec3& other) {
        std::transform(m_data, m_data + 3, other.m_data, m_data, std::divides<float>());

        return *this;
    }

    vec3& operator+=(float scalar) {
        for (float& e : m_data) { e += scalar; }
        return *this;
    }

    vec3& operator-=(float scalar) {
        for (float& e : m_data) { e -= scalar; }
        return *this;
    }

    vec3& operator*=(float scalar) {
        for (float& e : m_data) { e *= scalar; }
        return *this;
    }

    vec3& operator/=(float scalar) {
        for (float& e : m_data) { e /= scalar; }
        return *this;
    }

    float norm() const {
        return sqrtf(x*x + y*y + z*z);
    }

    void normalize() {
        operator/=(norm());
    }


};

inline vec3 normalize(vec3 v) {
    v.normalize();
    return v;
}

inline float dot(const vec3& lhs, const vec3& rhs) {
    return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}

inline vec3 cross(const vec3& lhs, const vec3& rhs) {
    return vec3{lhs.y*rhs.z - lhs.z*rhs.y,
                lhs.z*rhs.x - lhs.x*rhs.z,
                lhs.x*rhs.y - lhs.y*rhs.x};
}

inline std::ostream& operator<<(std::ostream& os, const vec3& v) {
    for (const float& e : v.m_data) {
        os << e << ' ';
    }

    return os;
}

inline vec3 operator+(vec3 lhs, const vec3& rhs) {
    return lhs += rhs;
}

inline vec3 operator-(vec3 lhs, const vec3& rhs) {
    return lhs -= rhs;
}

inline vec3 operator*(vec3 lhs, const vec3& rhs) {
    return lhs *= rhs;
}

inline vec3 operator/(vec3 lhs, const vec3& rhs) {
    return lhs /= rhs;
}

inline vec3 operator*(vec3 lhs, float scalar) {
    return lhs *= scalar;
}

#endif // vec3_H
