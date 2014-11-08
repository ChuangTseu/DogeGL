#include "texture.h"

#include <iostream>

Texture::Texture()
{
    glGenTextures(1, &m_tex);
}

void Texture::bindToTarget(GLuint target)
{
    glActiveTexture(target);
    glBindTexture(GL_TEXTURE_2D, m_tex);
}

bool Texture::loadFromFile(std::string filename)
{
    Image image;

    if (!image.loadFromFile(filename))
    {
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, m_tex);

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

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, image.getWidth(), image.getHeight(), 0, formatFrom, GL_UNSIGNED_BYTE, image.getData());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}


bool Texture::loadEmpty(int width, int height, TargetType type)
{
    glBindTexture(GL_TEXTURE_2D, m_tex);

//    GLuint pixelDataType;

//    if (gl_format == GL_DEPTH_COMPONENT24) {
//        pixelDataType = GL_FLOAT;
//    }

    if (type == TargetType::COLOR) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }
    else if (type == TargetType::DEPTH) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else {
        std::cerr << "Wrong TargetType type for empty texture \n";

        return false;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    return true;
}
