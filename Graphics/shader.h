#ifndef SHADER_H
#define SHADER_H

#include <algorithm>
#include <iostream>

#include "GL.h"

#include "MathsTools/mat4.h"



class Shader {
    GLuint m_program;
    GLuint m_vertexShader;
    GLuint m_fragmentShader;

public:
    Shader() {
        m_program = glCreateProgram();
    }

    ~Shader() {
        glDeleteProgram(m_program);
    }

    static void unbind() {
        glUseProgram(0);
    }

    bool addVertexShader(std::string filename);
    bool addFragmentShader(std::string filename);
    bool addTessControlShader(std::string filename);
    bool addTessEvaluationShader(std::string filename);
    bool addGeometryShader(std::string filename);

    bool link();

    void use();

    GLuint getProgramId() const {
        return m_program;
    }

    void sendTransformations(const mat4 &projection, const mat4 &view, const mat4 &model);

    void renew() {
        glDeleteProgram(m_program);
        m_program = glCreateProgram();
    }

private:
    bool addShader(std::string filename, GLenum type);
};

#endif // SHADER_H
