#ifndef SKYBOX_H
#define SKYBOX_H

#include "GL.h"
#include "cubemap.h"
#include "shader.h"
#include "model.h"

#include "../MathsTools/mat4.h"

class Skybox
{
    Cubemap m_cubemap;

    Shader m_skyboxShader;

    Model m_cube;

public:
    Skybox() {
        m_cube.loadFromFile("cube.obj");

        m_skyboxShader.addVertexShader("skybox.vert");
        m_skyboxShader.addFragmentShader("skybox.frag");
        m_skyboxShader.link();
    }

    void feedCubemap(const Cubemap& cubemap) {
        m_cubemap = cubemap;
    }

    Shader& getShader() {
        return m_skyboxShader;
    }

    void render(const mat4& projection, const mat4& pureView) {
        glDepthMask(GL_FALSE);

        m_skyboxShader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap.getId());

        mat4 PureViewProjection = projection * pureView;

        glUniformMatrix4fv(glGetUniformLocation(m_skyboxShader.getProgramId(), "PureViewProjection"), 1, GL_FALSE,
                           PureViewProjection.data());

        m_cube.drawAsTriangles();

        Shader::unbind();
        glDepthMask(GL_TRUE);
    }
};

#endif // SKYBOX_H
