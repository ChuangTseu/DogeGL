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

void MainWindow::reloadShader()
{
    m_oglWidget->reloadShader();
}

void MainWindow::toggleWireframe()
{
    m_oglWidget->toggleWireframe();
}

void MainWindow::setTargetStandard()
{
    setFinalFboTarget(0);

    uncheckTargetActionsAll();
    ui->actionStandard->setChecked(true);
}

void MainWindow::setTargetNormal()
{
    setFinalFboTarget(1);

    uncheckTargetActionsAll();
    ui->actionNormal->setChecked(true);
}

void MainWindow::setTargetTexcoord()
{
    setFinalFboTarget(2);

    uncheckTargetActionsAll();
    ui->actionTexcoord->setChecked(true);
}

void MainWindow::setTargetDepth()
{
    setFinalFboTarget(3);

    uncheckTargetActionsAll();
    ui->actionDepth->setChecked(true);
}

void MainWindow::setFinalFboTarget(int targetIndex)
{
    m_oglWidget->setFinalFboTarget(targetIndex);
}

void MainWindow::uncheckTargetActionsAll()
{
    ui->actionStandard->setChecked(false);
    ui->actionNormal->setChecked(false);
    ui->actionTexcoord->setChecked(false);
    ui->actionDepth->setChecked(false);
}
