#ifndef SHADER_H
#define SHADER_H

#include <algorithm>
#include <iostream>

#include "GL.h"

#include "MathsTools/mat4.h"

#include "material.h"

#include <string>
#include <memory>



class Shader {
    enum { VERTEX_SHADER = 0, TESS_CONTROL_SHADER, TESS_EVAL_SHADER, GEOMETRY_SHADER, FRAGMENT_SHADER, NUM_SHADERS_STAGES };

    GLuint m_program;
    GLuint m_vertexShader;
    GLuint m_fragmentShader;

    std::unique_ptr<std::string> m_currentShaderFilenames[NUM_SHADERS_STAGES] = {nullptr};

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

    void sendMaterial(const Material& mat);

    void reload() {
        renew();

        if (m_currentShaderFilenames[VERTEX_SHADER]) addVertexShader(*(m_currentShaderFilenames[VERTEX_SHADER]));
        if (m_currentShaderFilenames[TESS_CONTROL_SHADER]) addTessControlShader(*(m_currentShaderFilenames[TESS_CONTROL_SHADER]));
        if (m_currentShaderFilenames[TESS_EVAL_SHADER]) addTessEvaluationShader(*(m_currentShaderFilenames[TESS_EVAL_SHADER]));
        if (m_currentShaderFilenames[GEOMETRY_SHADER]) addGeometryShader(*(m_currentShaderFilenames[GEOMETRY_SHADER]));
        if (m_currentShaderFilenames[FRAGMENT_SHADER]) addFragmentShader(*(m_currentShaderFilenames[FRAGMENT_SHADER]));

        link();
    }

    void renew() {
        glDeleteProgram(m_program);
        m_program = glCreateProgram();
    }

private:
    bool addShader(std::string filename, GLenum type);
};

#endif // SHADER_H
