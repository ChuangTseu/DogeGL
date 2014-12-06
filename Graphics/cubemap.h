#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "GL.h"

#include "image.h"

#include <string>

class Cubemap
{
    GLuint m_tex;

public:
    Cubemap() {
        glGenTextures(1, &m_tex);
    }

    ~Cubemap() {
        glDeleteTextures(1, &m_tex);
    }

    enum Face {
        POSITIVE_X = 0, NEGATIVE_X, POSITIVE_Y, NEGATIVE_Y, POSITIVE_Z, NEGATIVE_Z
    };

    bool loadFaceFromFile(Face face, std::string filename) {
        Image image;

        if (!image.loadFromFile(filename, true))
        {
            return false;
        }

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_tex);

        GLuint formatFrom;
        switch(image.getBytesPerPixel()) {
        case 1:
            formatFrom = GL_RED;
            break;
        case 3:
            formatFrom = GL_RGB;
            break;
        case 4:
            formatFrom = GL_RGBA;
            break;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB,
                     image.getWidth(), image.getHeight(), 0, formatFrom, GL_UNSIGNED_BYTE, image.getData());

//        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        return true;
    }

    GLuint getId() const {
        return m_tex;
    }
};

#endif // CUBEMAP_H
