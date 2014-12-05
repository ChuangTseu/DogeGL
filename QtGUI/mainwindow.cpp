#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "openglwidget.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_oglWidget = new OpenGLWidget(this);
    setCentralWidget(m_oglWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setRenderer(RendererInterface *renderer) {
    m_oglWidget->setRenderer(renderer);
}

void MainWindow::loadModel()
{
    m_oglWidget->loadModel();
}
