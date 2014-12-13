#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QGLWidget>

#include <QPoint>

#include "rendererinterface.h"

class Renderer;

class QTimer;

class MainWindow;

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

private:
    MainWindow *getMainWindow();

signals:

public slots:
    void loadModel();

    void reloadShader();

    void toggleWireframe();

    void setFinalFboTarget(int targetIndex);

    void gammaChanged(float value);

    void keyValueChanged(double value);

private:
    RendererInterface* m_renderer;

    QTimer* m_realTimeTimer;

    QPoint m_lastMousePos;
};

#endif // OPENGLWIDGET_H
