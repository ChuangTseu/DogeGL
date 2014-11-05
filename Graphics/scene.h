#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <SDL.h>
#include <iostream>
#include <string>
#include "GL.h"

#include "shader.h"
#include "camera.h"
#include "model.h"

class Scene
{    
    std::string m_windowTitle = "FirstWindow";
    int m_windowWidth;
    int m_windowHeight;
    int m_type;

    SDL_Window* m_window;
    SDL_GLContext m_openGLContext;

public:
    Scene(int width, int height, int type = WINDOWED);

    enum {
        FULLSCREEN, WINDOWED
    };

    bool initWindow();
    bool initGL();
    void mainLoop();

    int getWindowWidth() const {
        return m_windowWidth;
    }

    int getWindowHeight() const {
        return m_windowHeight;
    }

    SDL_Window* getSDL_Window() {
        return m_window;
    }

    std::string getWindowTitle() const {
        return m_windowTitle;
    }

    bool bob();
};

#endif // SCENE_H
