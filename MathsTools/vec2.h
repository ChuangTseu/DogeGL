#ifndef VEC2_H
#define VEC2_H

#include <algorithm>
#include <iostream>
#include <functional>

struct vec2
{
    enum { N = 2 };

    union {
        float m_data[N];

        struct {
            float x;
            float y;
        };
    };


public:
    float* data() {
        return (float*) m_data;
    }

    const float* data() const {
        return (float*)m_data;
    }

    vec2& operator+=(const vec2& other) {
        std::transform(m_data, m_data + N, other.m_data, m_data, std::plus<float>());

        return *this;
    }

    vec2& operator-=(const vec2& other) {
        std::transform(m_data, m_data + N, other.m_data, m_data, std::minus<float>());

        return *this;
    }

    vec2& operator*=(const vec2& other) {
        std::transform(m_data, m_data + N, other.m_data, m_data, std::multiplies<float>());

        return *this;
    }

    vec2& operator/=(const vec2& other) {
        std::transform(m_data, m_data + N, other.m_data, m_data, std::divides<float>());

        return *this;
    }

    vec2& operator+=(float scalar) {
        for (float& e : m_data) { e += scalar; }
        return *this;
    }

    vec2& operator-=(float scalar) {
        for (float& e : m_data) { e -= scalar; }
        return *this;
    }

    vec2& operator*=(float scalar) {
        for (float& e : m_data) { e *= scalar; }
        return *this;
    }

    vec2& operator/=(float scalar) {
        for (float& e : m_data) { e /= scalar; }
        return *this;
    }

    float norm() const {
        return sqrtf(x*x + y*y);
    }

    void normalize() {
        operator/=(norm());
    }

};

inline float dot(const vec2& lhs, const vec2& rhs) {
    return lhs.x*rhs.x + lhs.y*rhs.y;
}

inline std::ostream& operator<<(std::ostream& os, const vec2& v) {
    for (const float& e : v.m_data) {
        os << e << ' ';
    }

    return os;
}

inline vec2 operator+(vec2 lhs, const vec2& rhs) {
    return lhs += rhs;
}

inline vec2 operator-(vec2 lhs, const vec2& rhs) {
    return lhs -= rhs;
}

inline vec2 operator*(vec2 lhs, const vec2& rhs) {
    return lhs *= rhs;
}

inline vec2 operator/(vec2 lhs, const vec2& rhs) {
    return lhs /= rhs;
}

#endif // VEC2_H
