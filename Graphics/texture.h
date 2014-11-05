#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL.h"

#include "image.h"

#include <string>

class Texture
{
    GLuint m_tex;

public:
    Texture();

    void bindToTarget(GLuint target);

    bool loadFromFile(std::string filename);
    bool loadEmpty(int width, int height, GLuint gl_format);

    GLuint getId() const {
        return m_tex;
    }
};

#endif // TEXTURE_H
