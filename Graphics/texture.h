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

    ~Texture();

    enum class TargetType {
        COLOR, DEPTH
    };

    void bindToTarget(GLuint target) const;

    bool loadFromFile(std::string filename, GLuint internalFormat = GL_RGB);
    bool loadEmpty(int width, int height, TargetType type, GLuint channelsType = GL_RGBA);

    bool loadFromMaterialColor(Color3f matColor);

    GLuint getId() const {
        return m_tex;
    }
};

static inline int channelToRgbFormat(int numChannels) {
    switch(numChannels) {
    case 1:
        return GL_RED;
    case 3:
        return GL_RGB16F;
    case 4:
        return GL_RGBA16F;
    default:
        std::cerr << "Channel number " << numChannels << " not handled. Return -1" << '\n';
        return -1;
    }
}

static inline int channelToSRgbFormat(int numChannels) {
    switch(numChannels) {
    case 3:
        return GL_SRGB;
    case 4:
        return GL_SRGB_ALPHA;
    default:
        std::cerr << "Channel number " << numChannels << " not handled. Return -1" << '\n';
        return -1;
    }
}

class NewTexture
{
    GLuint m_tex;

    int m_width;
    int m_height;

public:
    NewTexture() {
        glGenTextures(1, &m_tex);
    }

    ~NewTexture() {
        glDeleteTextures(1, &m_tex);
    }

    enum class TargetType {
        COLOR, DEPTH
    };

    void bindToTarget(GLuint target) const {
        glActiveTexture(target);
        glBindTexture(GL_TEXTURE_2D, m_tex);
    }

    bool loadFromFile(std::string filename) {
        Image image;

        if (!image.loadFromFile(filename))
        {
            return false;
        }

        glBindTexture(GL_TEXTURE_2D, m_tex);

        GLuint formatFrom = image.getGlFormat();
        GLuint typeFrom = image.getGlType();

        m_width = image.getWidth();
        m_height = image.getHeight();

        GLuint internalFormat;

        if (image.isHdr()) {
            std::cerr << "HDR!\n";
            internalFormat = channelToRgbFormat(image.getNumChannels());
        }
        else {
            internalFormat = channelToSRgbFormat(image.getNumChannels());
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.getWidth(), image.getHeight(), 0, formatFrom, typeFrom, image.getData());

    //    for (int i = 0; i < image.getWidth()*image.getHeight(); ++i) {
    //        std::cout << ((float*)image.getData())[i] << "  ";
    //    }
        //        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        return true;
    }

    GLuint getId() const {
        return m_tex;
    }

    int height() const;
    void setHeight(int height);
    int width() const;
    void setWidth(int width);
};

#endif // TEXTURE_H
