#include "mainwindow.h"

#include "openglwidget.h"

#include <iostream>

MainWindow::MainWindow(std::string title, int width, int height, int type) :
    m_title(title),
    m_width(width),
    m_height(height),
    m_type(type)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
        SDL_Quit();

        exit(EXIT_FAILURE);
    }

    // OpenGL version

//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    // Double Buffer

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Window

    Uint32 windowAttributesFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

    if (m_type == FULLSCREEN) {
        windowAttributesFlags |= SDL_WINDOW_FULLSCREEN;
    }

    m_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                m_width, m_height, windowAttributesFlags);

    if(!m_window)
    {
        std::cout << "SDL Error : " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    //OpenGL context

    m_openGLContext = SDL_GL_CreateContext(m_window);

    if(m_openGLContext == 0)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();

        exit(EXIT_FAILURE);
    }

    m_oglWidget = new OpenGLWidget();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setRenderer(RendererInterface *renderer) {
    m_oglWidget->setRenderer(renderer);

    m_oglWidget->initializeGL();
}

void MainWindow::runLoop()
{
    // Pure FPS Mode
    SDL_SetRelativeMouseMode(SDL_TRUE);

    while(!m_input.updateEvents() == Input::QUIT_EVENT)
    {
        // WARNING: Following content rated mature, keep children far from this terrible stuff
        int mouse_x_rel = m_input.getXRel();
        int mouse_y_rel = m_input.getYRel();

        if (m_input.getKey(SDL_SCANCODE_W)) {
            m_oglWidget->keyPressEvent(Qt::Key_Z);        }
        if (m_input.getKey(SDL_SCANCODE_S)) {
            m_oglWidget->keyPressEvent(Qt::Key_S);       }
        if (m_input.getKey(SDL_SCANCODE_A)) {
            m_oglWidget->keyPressEvent(Qt::Key_Q);       }
        if (m_input.getKey(SDL_SCANCODE_D)) {
            m_oglWidget->keyPressEvent(Qt::Key_D);        }
        if (m_input.getKey(SDL_SCANCODE_LSHIFT)) {
            m_oglWidget->keyPressEvent(Qt::Key_Shift);       }
        if (m_input.getKey(SDL_SCANCODE_LCTRL)) {
            m_oglWidget->keyPressEvent(Qt::Key_Control);        }

        m_oglWidget->mouseMoveEvent(mouse_x_rel, mouse_y_rel);


        if (m_input.getKey(SDL_SCANCODE_R)) {
            m_oglWidget->reloadShader();
        }

        if (m_input.getKey(SDL_SCANCODE_KP_PLUS)) {
            m_oglWidget->keyPressEvent(Qt::Key_Plus);
        }
        else if (m_input.getKey(SDL_SCANCODE_KP_MINUS)) {
            m_oglWidget->keyPressEvent(Qt::Key_Minus);
        }

        if (m_input.getKey(SDL_SCANCODE_Z)) {
            m_oglWidget->toggleWireframe();
        }

        if (m_input.getKey(SDL_SCANCODE_KP_0)) {
            m_oglWidget->setFinalFboTarget(0);      }
        else if (m_input.getKey(SDL_SCANCODE_KP_1)) {
            m_oglWidget->setFinalFboTarget(1);      }
        else if (m_input.getKey(SDL_SCANCODE_KP_2)) {
            m_oglWidget->setFinalFboTarget(2);      }
        else if (m_input.getKey(SDL_SCANCODE_KP_3)) {
            m_oglWidget->setFinalFboTarget(3);      }
        else if (m_input.getKey(SDL_SCANCODE_KP_4)) {
            m_oglWidget->setFinalFboTarget(4);      }
        else if (m_input.getKey(SDL_SCANCODE_KP_5)) {
            m_oglWidget->setFinalFboTarget(5);      }
        else if (m_input.getKey(SDL_SCANCODE_KP_6)) {
            m_oglWidget->setFinalFboTarget(6);      }




        m_oglWidget->paintGL();

        /* SWAP BUFFERS */
        SDL_GL_SwapWindow(m_window);

        /* ARBITRARILY LIMIT THE FRAMERATE, WE DON'T NEED OUR GPU TO CRY RENDERING A DUMB CUBE */
        SDL_Delay(8);
    }

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
}

void MainWindow::setTargetNormal()
{
    setFinalFboTarget(1);
}

void MainWindow::setTargetTexcoord()
{
    setFinalFboTarget(2);
}

void MainWindow::setTargetDepth()
{
    setFinalFboTarget(3);
}

void MainWindow::setFinalFboTarget(int targetIndex)
{
    m_oglWidget->setFinalFboTarget(targetIndex);
}
