#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QGLWidget>

#include "rendererinterface.h"

class Renderer;

class QTimer;

class OpenGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = 0);
    virtual ~OpenGLWidget();

    void setRenderer(RendererInterface* renderer) {
        m_renderer = renderer;
    }

protected:

    void initializeGL(void);
    void resizeGL(int width, int height);
    void paintGL(void);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void wheelEvent(QWheelEvent* event);

    void keyPressEvent(QKeyEvent* event);

signals:

public slots:
    void loadModel();

    void reloadShader();

private:
    RendererInterface* m_renderer;

    QTimer* m_realTimeTimer;
};

#endif // OPENGLWIDGET_H
