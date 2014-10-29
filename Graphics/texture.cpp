#include "texture.h"

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
    if (!image.loadFromFile(filename))
    {
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, m_tex);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.getData());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}
