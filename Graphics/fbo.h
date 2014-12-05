#ifndef FBO_H
#define FBO_H

#include "GL.h"

#include "texture.h"
#include <vector>
#include <iostream>

class FBO
{
    GLuint m_fboId;

    std::vector<Texture> m_colorTextures;
    Texture m_depth;

    GLuint m_depthRenderBufferId;

    size_t m_width;
    size_t m_height;

public:
    FBO(int width, int height, int numColorTextures = 1, bool useTextureForDepth = true);

    ~FBO();

    GLuint getId() const {
        return m_fboId;
    }

    size_t getWidth() const {
        return m_width;
    }

    size_t getHeight() const {
        return m_height;
    }

    Texture& getTexture(unsigned int id) {
        if (id >= m_colorTextures.size()) {
            std::cerr << "WARNING: trying to access wrong fbo texture. Sending back the default one (0).\n";

            id = 0;
        }

        return m_colorTextures[id];
    }

    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
    }

    static void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};

#endif // FBO_H
