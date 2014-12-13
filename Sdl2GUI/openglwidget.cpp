#include "openglwidget.h"

#include <iostream>

OpenGLWidget::OpenGLWidget()
{
}

OpenGLWidget::~OpenGLWidget()
{

}


void OpenGLWidget::initializeGL(void) {
    m_renderer->initializeGL();
}

void OpenGLWidget::resizeGL(int width, int height) {
    m_renderer->resizeGL(width, height);
}

void OpenGLWidget::paintGL(void) {
    m_renderer->paintGL();
}

void OpenGLWidget::mousePressEvent(void* event) {
//    m_lastMousePos.rx() = event->x();
//    m_lastMousePos.ry() = event->y();
}

void OpenGLWidget::mouseMoveEvent(int mouse_x_rel, int mouse_y_rel) {
    m_renderer->rotateCamera(mouse_x_rel, mouse_y_rel);
}

void OpenGLWidget::mouseReleaseEvent(void* event) {

}

void OpenGLWidget::wheelEvent(void* event) {

}

void OpenGLWidget::keyPressEvent(int qt_key)
{
    m_renderer->onKeyPress(qt_key);
}

void OpenGLWidget::loadModel()
{
    std::cerr << "Loading model... \n";

//    QString filename = QFileDialog::getOpenFileName();

//    m_renderer->loadModel(filename.toStdString());
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
