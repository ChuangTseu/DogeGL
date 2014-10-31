#include "image.h"

#include <iostream>

Image::Image()
{
}

bool Image::loadFromFile(std::string filename)
{
    ilInit();

    ILuint imageId; // The image name to return.
    ilGenImages(1, &imageId); // Grab a new image name.

    ilBindImage(imageId);

    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    if (ilLoadImage(filename.c_str()) != IL_TRUE) {
        std::cerr << "Error loading image from: " << filename << '\n';
        return false;
    }

    m_data = ilGetData();

    m_width = ilGetInteger(IL_IMAGE_WIDTH);
    m_height = ilGetInteger(IL_IMAGE_HEIGHT);

    return true;
}
