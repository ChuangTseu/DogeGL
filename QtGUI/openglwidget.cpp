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
    m_lastMousePos.rx() = event->x();
    m_lastMousePos.ry() = event->y();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
    QPoint currentMousePos(event->x(), event->y());


    QPoint diff = currentMousePos - m_lastMousePos;

    if (event->modifiers() & Qt::ControlModifier) {
        m_renderer->translateCamera(diff.x(), diff.y(), 0);
    }
    else if (event->modifiers() & Qt::ShiftModifier) {
        m_renderer->translateCamera(0, 0, -diff.y());
    }
    else {
        m_renderer->rotateCamera(diff.x(), diff.y());
    }

    m_lastMousePos = currentMousePos;
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event) {

}

void OpenGLWidget::wheelEvent(QWheelEvent* event) {
    m_renderer->translateCamera(0, 0, event->delta() / 10);
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
