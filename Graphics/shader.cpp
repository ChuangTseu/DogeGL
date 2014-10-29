#include "shader.h"

#include "../helpers.h"

bool Shader::addVertexShader(std::string filename) {
    return addShader(filename, GL_VERTEX_SHADER);
}

bool Shader::addFragmentShader(std::string filename) {
    return addShader(filename, GL_FRAGMENT_SHADER);
}

bool Shader::addTessControlShader(std::string filename) {
    return addShader(filename, GL_TESS_CONTROL_SHADER);
}

bool Shader::addTessEvaluationShader(std::string filename) {
    return addShader(filename, GL_TESS_EVALUATION_SHADER);
}

bool Shader::addShader(std::string filename, GLenum type) {
    GLint length;
    GLchar* source = readFile(filename, &length);

    if (!source) {
        std::cerr << "Error reading source from " << filename;
        return false;
    }

    GLint shader_compiled;

    GLuint shaderId = glCreateShader(type);

    glShaderSource(shaderId, 1, &source, &length);

    free(source);

    glCompileShader(shaderId);

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shader_compiled);


    if (shader_compiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(shaderId, 1024, &log_length, message);
        // Write the error to a log
        std::cerr << "Error compiling shader " << filename << ": " << message;

        return false;
    }

    if (type == GL_VERTEX_SHADER) {
        // Using layout(location = x) instead

//        glBindAttribLocation(m_program, 0, "position");
//        glBindAttribLocation(m_program, 1, "normal");
    }
//    glBindAttribLocation(program, 0, "position"); // The index passed into glBindAttribLocation is
//    glBindAttribLocation(program, 1, "texcoord"); // used by glEnableVertexAttribArray. "position"
//    glBindAttribLocation(program, 2, "normal");   // "texcoord" "normal" and "color" are the names of the
//    glBindAttribLocation(program, 3, "color");    // respective inputs in your fragment shader.

    glAttachShader(m_program, shaderId);

    return true;
}

bool Shader::link() {
    glLinkProgram(m_program);

    GLint program_linked;
    glGetProgramiv(m_program, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(m_program, 1024, &log_length, message);
        // Write the error to a log
        std::cerr << "Error linking program: " << message;

        return false;
    }

    return true;
}

void Shader::use() {
    glUseProgram(m_program);
}

void Shader::sendTransformations(const mat4& projection, const mat4& view, const mat4& model) {
    mat4 MVP = projection * view * model;

    glUniformMatrix4fv(glGetUniformLocation(m_program, "MVP"), 1, GL_FALSE, MVP.data());

    glUniformMatrix4fv(glGetUniformLocation(m_program, "world"), 1, GL_FALSE, model.data());

//    glUniformMatrix4fv(glGetUniformLocation(m_program, "projection"), 1, GL_FALSE, projection.data());
//    glUniformMatrix4fv(glGetUniformLocation(m_program, "modelview"), 1, GL_FALSE, view.data());
//    glUniformMatrix4fv(glGetUniformLocation(m_program, "model"), 1, GL_FALSE, model.data());
}
