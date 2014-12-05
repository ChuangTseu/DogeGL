#ifndef RENDERERINTERFACE_H
#define RENDERERINTERFACE_H

#include <string>

#include <QtCore/qnamespace.h>

class RendererInterface
{
public:
    RendererInterface() {

    }

    virtual void initializeGL(void) = 0;
    virtual void resizeGL(int width, int height) = 0;
    virtual void paintGL(void) = 0;

    virtual void loadModel(const std::string& filename) = 0;

    virtual void onKeyPress(int qt_key) = 0;

    virtual void reloadShader() = 0;
};

#endif // RENDERERINTERFACE_H
