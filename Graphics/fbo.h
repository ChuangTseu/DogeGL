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

public:
    FBO(int width, int height, int numColorTextures = 1, bool useTextureForDepth = true) {
        glGenFramebuffers(1, &m_fboId);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);

        m_colorTextures.reserve(numColorTextures + useTextureForDepth);

        for (int i = 0; i < numColorTextures; ++i) {
            m_colorTextures.emplace_back();

            m_colorTextures[i].loadEmpty(width, height, Texture::TargetType::COLOR);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorTextures[i].getId(), 0);
        }

        if (useTextureForDepth) {
            m_colorTextures.emplace_back();

            m_colorTextures[numColorTextures].loadEmpty(width, height, Texture::TargetType::DEPTH);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_colorTextures[numColorTextures].getId(), 0);
        }
        else {
            glGenRenderbuffers(1, &m_depthRenderBufferId);
            glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBufferId);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBufferId);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Error. Framebuffer INCOMPLETE." << '\n';
        }


    }

    GLuint getId() const {
        return m_fboId;
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

    ~FBO() {
        glDeleteFramebuffers(1, &m_fboId);
    }
};

#endif // FBO_H
