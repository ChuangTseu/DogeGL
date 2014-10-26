#ifndef MAT4_H
#define MAT4_H

#include <algorithm>
#include <iostream>

#include "vec4.h"

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

#endif // MAT4_H
