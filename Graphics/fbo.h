#ifndef FBO_H
#define FBO_H

#include "GL.h"

#include "texture.h"
#include <vector>
#include <iostream>

#include "Utils/codegenerators.h"

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

class SingleColorFBO {
    SafeGlFboId m_fboId;
    SafeGlTexId m_texId;

    int m_width;
    int m_height;

public:
    GENERATE_FiveDefaultsRule(SingleColorFBO)

    SingleColorFBO(int width, int height, GLint filterParam = GL_LINEAR) :
        m_width(width),
        m_height(height){
//        std::cerr << "Building fbo " << m_width << " x " << m_height << '\n';

        glBindFramebuffer(GL_FRAMEBUFFER, m_fboId.get());

        glBindTexture(GL_TEXTURE_2D, m_texId.get());

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        if (filterParam == GL_LINEAR) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else if(filterParam == GL_NEAREST) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        else {
            std::cerr << "Error. SingleColorFBO filterParam UNKNOWN/FALSE." << '\n';
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texId.get(), 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Error. SingleColorFBO framebuffer INCOMPLETE." << '\n';
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboId.get());
    }

    static void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void bindToTarget(GLuint target) const
    {
        glActiveTexture(target);
        glBindTexture(GL_TEXTURE_2D, m_texId.get());
    }
};

#endif // FBO_H
