#ifndef SHADER_H
#define SHADER_H

#include <algorithm>
#include <iostream>

#include <GL/glew.h>



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

    bool addVertexShader(std::string filename);
    bool addFragmentShader(std::string filename);

    bool link();

    void use();

    GLuint getProgramId() const {
        return m_program;
    }

private:
    bool addShader(std::string filename, GLenum type);
};

#endif // SHADER_H
