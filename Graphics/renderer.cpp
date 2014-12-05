#include "renderer.h"

#include <iostream>
#include <string>

#include "GL.h"

#include "orthobase.h"
#include "texture.h"
#include "light.h"
#include "shader.h"
#include "camera.h"
#include "fbo.h"
#include "cubemap.h"
#include "skybox.h"
#include "shadowmap.h"

Renderer::Renderer(int width, int height) :
    m_width(width),
    m_height(height)
{
    std::cerr << "WIDTHxHEIGHT: " << width << "x" << height << '\n';
}

void Renderer::initializeGL(void) {
    initGL();

    glViewport(0, 0, m_width, m_height);

    m_scene = new Scene(m_width, m_height);
    m_scene->initScene();
}

void Renderer::resizeGL(int width, int height) {

}

void Renderer::paintGL(void) {
    render();
}

bool Renderer::initGL()
{
    std::cerr << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cerr << "OpenGL Vendor : " << glGetString(GL_VENDOR) << std::endl;


    // EVERYONE ON EVERY PLATFORM WILL ENJOY THIS
//    #ifdef WIN32

    // On initialise GLEW

    /*Potentiel fix pour le crash quand SDL contexte > OpenGL 3.1 + GLEW*/
    glewExperimental = GL_TRUE;

    GLenum GLEWinitialization( glewInit() );

    std::cerr << "Initializing GLEW...\n";

    if(GLEWinitialization != GLEW_OK)
    {
        // On affiche l'erreur gr�ce � la fonction : glewGetErrorString(GLenum code)

        std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString(GLEWinitialization) << std::endl;


        // On quitte la SDL

//        SDL_GL_DeleteContext(m_openGLContext);
//        SDL_DestroyWindow(m_window);
//        SDL_Quit();

        return false;
    }

    if (GLEW_ARB_debug_output)
    {
//        glDebugMessageCallbackARB(&glBreak_debug, nullptr);
    }


//    #endif

    // Activation du Depth Buffer

    glEnable(GL_DEPTH_TEST);

    /*glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);*/

    return true;
}

void Renderer::render()
{
    m_scene->render();
}
