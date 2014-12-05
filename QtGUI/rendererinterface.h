#ifndef RENDERERINTERFACE_H
#define RENDERERINTERFACE_H

class RendererInterface
{
public:
    RendererInterface() {

    }

    virtual void initializeGL(void) = 0;
    virtual void resizeGL(int width, int height) = 0;
    virtual void paintGL(void) = 0;
};

#endif // RENDERERINTERFACE_H
