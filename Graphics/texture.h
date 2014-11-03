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
};

#endif // TEXTURE_H
