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
    int m_windowWidth = 640;
    int m_windowHeight = 480;

    SDL_Window* m_window;
    SDL_GLContext m_openGLContext;

public:
    Scene();

    bool initWindow();
    bool initGL();
    void mainLoop();

    bool bob();
};

#endif // SCENE_H
