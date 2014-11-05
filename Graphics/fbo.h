#ifndef FBO_H
#define FBO_H

#include "GL.h"

#include "texture.h"
#include <iostream>

class FBO
{
    GLuint m_fboId;

    Texture m_tex;
    Texture m_depth;

    GLuint m_depthRenderBufferId;

public:
    FBO() {
        glGenFramebuffers(1, &m_fboId);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);

        m_tex.loadEmpty(640, 480, GL_RGBA);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex.getId(), 0);


        glGenRenderbuffers(1, &m_depthRenderBufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 640, 480);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBufferId);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Error. Framebuffer INCOMPLETE." << '\n';
        }


    }

    GLuint getId() const {
        return m_fboId;
    }

    Texture& getTexture() {
        return m_tex;
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
