#ifndef RENDERER_H
#define RENDERER_H

#include "QtGUI/rendererinterface.h"

#include "scene.h"

class Renderer : public RendererInterface
{
public:
    Renderer(int width, int height);

    virtual void initializeGL(void);
    virtual void resizeGL(int width, int height);
    virtual void paintGL(void);

    bool initGL();
    void initScene();

    void render();


private:
    int m_width;
    int m_height;

//CURRENT RENDERLOOP DATA: TO REFACTOR !!!!
private:
    Scene* m_scene{nullptr};
};

#endif // RENDERER_H
