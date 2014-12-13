#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include "rendererinterface.h"

class Renderer;

class OpenGLWidget
{
public:
    explicit OpenGLWidget();
    virtual ~OpenGLWidget();

    void setRenderer(RendererInterface* renderer) {
        m_renderer = renderer;
    }

public:

    void initializeGL(void);
    void resizeGL(int width, int height);
    void paintGL(void);

    void mousePressEvent(void *event);
    void mouseMoveEvent(int mouse_x_rel, int mouse_y_rel);
    void mouseReleaseEvent(void *event);

    void wheelEvent(void *event);

    void keyPressEvent(int qt_key);

public:
    void loadModel();

    void reloadShader();

    void toggleWireframe();

    void setFinalFboTarget(int targetIndex);

private:
    RendererInterface* m_renderer;
};

#endif // OPENGLWIDGET_H
