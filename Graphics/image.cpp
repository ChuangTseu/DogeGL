#include "image.h"

#include <iostream>

Image::Image()
{
}

Image::~Image() {
    ilDeleteImage(m_imageId);

//    delete m_data;
}

bool Image::loadFromFile(std::string filename, bool reversed)
{
    ilInit();

     // The image name to return.
    ilGenImages(1, &m_imageId); // Grab a new image name.

    ilBindImage(m_imageId);

    ilEnable(IL_ORIGIN_SET);

    if (reversed)
        ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
    else
        ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    if (ilLoadImage(filename.c_str()) != IL_TRUE) {
        std::cerr << "Error loading image from: " << filename << '\n';
        return false;
    }

    m_data = ilGetData();

    m_width = ilGetInteger(IL_IMAGE_WIDTH);
    m_height = ilGetInteger(IL_IMAGE_HEIGHT);

    m_bytesPerPixel = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

    return true;
}
