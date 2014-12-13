#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <SDL.h>
#include <iostream>
#include <string>

#include "rendererinterface.h"

#include "input.h"

class OpenGLWidget;

class MainWindow
{
public:
    MainWindow(std::string title, int width, int height, int type = WINDOWED);

    ~MainWindow();

    void setRenderer(RendererInterface* renderer);

    void runLoop();

    enum {
        FULLSCREEN, WINDOWED
    };

    int getWindowWidth() const {
        return m_width;
    }

    int getWindowHeight() const {
        return m_height;
    }

    SDL_Window* getSDL_Window() {
        return m_window;
    }

    std::string getWindowTitle() const {
        return m_title;
    }

public:
    void loadModel();

    void reloadShader();

    void toggleWireframe();

    void setTargetStandard();
    void setTargetNormal();
    void setTargetTexcoord();
    void setTargetDepth();

private:
    void setFinalFboTarget(int targetIndex);

private:
    std::string m_title = "FirstWindow";
    int m_width;
    int m_height;
    int m_type;

    SDL_Window* m_window;
    SDL_GLContext m_openGLContext;

    OpenGLWidget* m_oglWidget;

    Input m_input;
};

#endif // MAINWINDOW_H
