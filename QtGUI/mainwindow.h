#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "rendererinterface.h"

class OpenGLWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setRenderer(RendererInterface* renderer);

signals:

public slots:
    void loadModel();

    void reloadShader();

    void toggleWireframe();

    void setTargetStandard();
    void setTargetNormal();
    void setTargetTexcoord();
    void setTargetDepth();

private:
    void setFinalFboTarget(int targetIndex);

    void uncheckTargetActionsAll();

private:
    Ui::MainWindow *ui;

    OpenGLWidget* m_oglWidget;
};

#endif // MAINWINDOW_H
