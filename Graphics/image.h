#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#include <memory>

#include "IL/il.h"

class Image
{
    unsigned int m_width;
    unsigned int m_height;

    unsigned char* m_data;

    unsigned int m_bytesPerPixel;

    int m_format;
    int m_type;
    int m_numChannels;

    ILuint m_imageId;

public:
    Image();

    ~Image();

    bool loadFromFile(std::string filename, bool reversed = false);

    unsigned int getWidth() const {
        return m_width;
    }

    unsigned int getHeight() const {
        return m_height;
    }

    unsigned int getBytesPerPixel() const {
        return m_bytesPerPixel;
    }

    int getNumChannels() const {
        return m_numChannels;
    }

    int getGlFormat() const {
        return m_format;
    }

    int getGlType() const {
        return m_type;
    }

    bool isHdr() const {
        return (m_type == IL_FLOAT);
    }

    const unsigned char* getData() const {
        return m_data;
    }
};

#endif // IMAGE_H
