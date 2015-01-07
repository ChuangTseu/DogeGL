#include "envmap.h"





EnvMap::EnvMap() {
    glGenTextures(1, &m_tex);

    m_cube.loadFromFile("cube.obj");

    m_shader.addVertexShader("skybox.vert");
    m_shader.addFragmentShader("skybox.frag");
    m_shader.link();
}

EnvMap::~EnvMap() {
    glDeleteTextures(1, &m_tex);
}

void EnvMap::renew() {
    glDeleteTextures(1, &m_tex);
    glGenTextures(1, &m_tex);

    m_shader.renew();
}

bool EnvMap::loadFromFile(std::string filename) {
    if (m_tex != 0) renew();

    Image image;

    if (!image.loadFromFile(filename))
    {
        return false;
    }

    int width = image.getWidth();
    int height = image.getHeight();

    int envType = determineTypeFromDimension(width, height);

    switch (envType) {
    case SPHERICAL:
        std::cerr << "Loading " << TypesStr[SPHERICAL] << " " << filename << " (" << image.getBytesPerPixel() << " bpp)" << '\n';
        loadAsSpherical(image);
        break;
    case CROSS_VERTICAL:
        std::cerr << "Loading " << TypesStr[CROSS_VERTICAL] << " " << filename << " (" << image.getBytesPerPixel() << " bpp)" << '\n';
        break;
    case CROSS_HORIZONTAL:
        std::cerr << "Loading " << TypesStr[CROSS_HORIZONTAL] << " " << filename << " (" << image.getBytesPerPixel() << " bpp)" << '\n';
        break;
    case LATLONG:
        std::cerr << "Loading " << TypesStr[LATLONG] << " " << filename << " (" << image.getBytesPerPixel() << " bpp)" << '\n';
        break;
    case LINE:
        std::cerr << "Loading " << TypesStr[LINE] << " " << filename << " (" << image.getBytesPerPixel() << " bpp)" << '\n';
        break;
    case UNKNOWN:
        std::cerr << "Error. Unknown environnement map type for " << filename << '\n';
        break;
    }

    return true;
}

void EnvMap::render(const mat4 &projection, const mat4 &pureView) {
    glDepthMask(GL_FALSE);

    m_shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    mat4 PureViewProjection = projection * pureView;

    glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramId(), "PureViewProjection"), 1, GL_FALSE,
                       PureViewProjection.data());

    m_cube.drawAsTriangles();

    Shader::unbind();

    glDepthMask(GL_TRUE);
}

int EnvMap::channelToRgbFormatHDR(int numChannels) const {
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

int EnvMap::channelToSRgbFormat(int numChannels) const {
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

bool EnvMap::loadAsSpherical(Image &image) {
    glBindTexture(GL_TEXTURE_2D, m_tex);

    GLuint formatFrom = image.getGlFormat();
    GLuint typeFrom = image.getGlType();

    GLuint internalFormat;

    if (image.isHdr()) {
        std::cerr << "HDR!\n";
        internalFormat = channelToRgbFormatHDR(image.getNumChannels());
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


    m_shader.addVertexShader("skybox.vert");
    m_shader.addFragmentShader("env_spherical.frag");

    m_shader.link();

    return true;
}

int EnvMap::determineTypeFromDimension(int width, int height) {
    int w = width;
    int h = height;

    if (w == h) {
        return SPHERICAL;
    }

    if (w == 2*h) {
        return LATLONG;
    }

    if (3*w == 4*h) {
        return CROSS_HORIZONTAL;
    }

    if (4*w == 3*h) {
        return CROSS_VERTICAL;
    }

    if (w == 6*h) {
        return LINE;
    }

    return UNKNOWN;
}
