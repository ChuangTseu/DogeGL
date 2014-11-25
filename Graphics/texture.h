#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL.h"

#include "image.h"

#include "MathsTools/vec3.h"

#include <string>

//#include "material.h"

class Texture
{
    GLuint m_tex;

public:
    Texture();

    enum class TargetType {
        COLOR, DEPTH
    };

    void bindToTarget(GLuint target) const;

    bool loadFromFile(std::string filename);
    bool loadEmpty(int width, int height, TargetType type);

    bool loadFromMaterialColor(Color3f matColor);

    GLuint getId() const {
        return m_tex;
    }
};

#endif // TEXTURE_H
