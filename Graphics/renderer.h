#ifndef RENDERER_H
#define RENDERER_H

#include "QtGUI/rendererinterface.h"

#include "scene.h"

#include <string>

class Renderer : public RendererInterface
{
public:
    Renderer(int width, int height);

    virtual void initializeGL(void);
    virtual void resizeGL(int width, int height);
    virtual void paintGL(void);

    virtual void loadModel(const std::string &filename);

    virtual void onKeyPress(int qt_key);

    virtual void reloadShader();
    virtual void toggleWireframe();
    virtual void setFinalFboTarget(int targetIndex);

    bool initGL();
    void initScene();

    void render();

    void rotateCamera(int mouse_x_rel, int mouse_y_rel);
    void translateCamera(int mouse_x_rel, int mouse_y_rel, int mouse_z_rel);

private:
    int m_width;
    int m_height;

//CURRENT RENDERLOOP DATA: TO REFACTOR !!!!
private:
    Scene* m_scene{nullptr};
};

#endif // RENDERER_H
