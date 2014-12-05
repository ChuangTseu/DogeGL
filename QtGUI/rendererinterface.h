#ifndef RENDERERINTERFACE_H
#define RENDERERINTERFACE_H

#include <string>

class RendererInterface
{
public:
    RendererInterface() {

    }

    virtual void initializeGL(void) = 0;
    virtual void resizeGL(int width, int height) = 0;
    virtual void paintGL(void) = 0;

    virtual void loadModel(const std::string& filename) = 0;
};

#endif // RENDERERINTERFACE_H
