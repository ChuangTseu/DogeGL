#include "openglwidget.h"

#include <iostream>

#include <QTimer>

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    QGLFormat fmt;
    fmt.setDoubleBuffer(true);
    fmt.setDirectRendering(true);
    fmt.setRgba(true);
    fmt.setStencil(false);
    fmt.setDepth(true);
    fmt.setAlpha(false);
    fmt.setVersion(4, 0);
    fmt.setProfile(QGLFormat::CoreProfile);
    QGLFormat::setDefaultFormat(fmt);

    if (!(QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_4_0)) {
        std::cerr << "Failed to create a valid Core Opengl 4.4 Profile. \n";
        exit(EXIT_FAILURE);
    }

    setFocusPolicy(Qt::StrongFocus);
    makeCurrent();
}

OpenGLWidget::~OpenGLWidget(void)
{

}


void OpenGLWidget::initializeGL(void) {
    m_renderer->initializeGL();

    m_realTimeTimer = new QTimer(this);
    m_realTimeTimer->setInterval(8);

    connect(m_realTimeTimer, SIGNAL(timeout()), this, SLOT(update()));

    m_realTimeTimer->start();
}

void OpenGLWidget::resizeGL(int width, int height) {

}

void OpenGLWidget::paintGL(void) {
    m_renderer->paintGL();
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event) {

}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event) {

}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event) {

}

void OpenGLWidget::wheelEvent(QWheelEvent* event) {

}
