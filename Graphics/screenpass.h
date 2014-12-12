#ifndef SCREENPASS_H
#define SCREENPASS_H

#include "GL.h"

#include "shader.h"
#include "mesh.h"

#include <string>

class ScreenPass
{
    Shader m_shader;

    Mesh m_screenQuad;

    int m_width;
    int m_height;

public:
    ScreenPass(int width, int height, std::string shaderFilename) :
        m_width(width),
        m_height(height)
    {
        m_shader.addFragmentShader(shaderFilename);
        m_shader.link();

        m_screenQuad.loadFullscreenQuad();
    }

    void resize(int width, int height) {
        m_width = width;
        m_height = height;
    }

    Shader& getShader() {
        return m_shader;
    }

    void fire() {
        GLint oldViewport[4];
        glGetIntegerv(GL_VIEWPORT, oldViewport);

        m_shader.use();

        glUniform1i(glGetUniformLocation(m_shader.getProgramId(), "screenWidth"), m_width);
        glUniform1i(glGetUniformLocation(m_shader.getProgramId(), "screenHeight"), m_height);

        glViewport(0, 0, m_width, m_height);

        m_screenQuad.draw();

        glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
    }
};

#endif // SCREENPASS_H
