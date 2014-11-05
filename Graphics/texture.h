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

    enum class TargetType {
        COLOR, DEPTH
    };

    void bindToTarget(GLuint target);

    bool loadFromFile(std::string filename);
    bool loadEmpty(int width, int height, TargetType type);

    GLuint getId() const {
        return m_tex;
    }
};

#endif // TEXTURE_H
