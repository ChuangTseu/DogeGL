#include "openglwidget.h"

#include <iostream>

#include <QTimer>
#include <QFileDialog>
#include <QKeyEvent>

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
    m_renderer->resizeGL(width, height);
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

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    m_renderer->onKeyPress(event->key());
}

void OpenGLWidget::loadModel()
{
    std::cerr << "Loading model... \n";

    QString filename = QFileDialog::getOpenFileName();

    m_renderer->loadModel(filename.toStdString());
}

void OpenGLWidget::reloadShader()
{
    m_renderer->reloadShader();
}

void OpenGLWidget::toggleWireframe()
{
    m_renderer->toggleWireframe();
}

void OpenGLWidget::setFinalFboTarget(int targetIndex)
{
    m_renderer->setFinalFboTarget(targetIndex);
}
