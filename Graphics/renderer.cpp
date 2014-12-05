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
}

void Renderer::initializeGL(void) {
    initGL();

    glViewport(0, 0, m_width, m_height);

    m_scene = new Scene(m_width, m_height);
    m_scene->initScene();

    m_scene->resize(m_width, m_height);
}

void Renderer::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);

    m_width = width;
    m_height = height;

    m_scene->resize(width, height);
}

void Renderer::paintGL(void) {
    render();
}

void Renderer::loadModel(const std::string &filename)
{
    m_scene->mainModel.loadFromFile(filename);
}

void Renderer::onKeyPress(int qt_key)
{
    vec3 kright = normalize(cross(m_scene->forward, m_scene->up));
    vec3 kdown = normalize(cross(m_scene->forward, kright));

//    forward += kright*(mouse_x_rel/100.f);
//    forward += kdown*(mouse_y_rel/100.f);

//    forward.normalize();

    if (qt_key == Qt::Key_Z /*Z*/) {
        m_scene->position += m_scene->forward*0.1f;        }
    if (qt_key == Qt::Key_S /*S*/) {
        m_scene->position -= m_scene->forward*0.1f;        }
    if (qt_key == Qt::Key_Q /*Q*/) {
        m_scene->position -= kright*0.1f;        }
    if (qt_key == Qt::Key_D /*D*/) {
        m_scene->position += kright*0.1f;        }
    if (qt_key == Qt::Key_Shift /*SHIFT*/) {
        m_scene->position += m_scene->up*0.1f;        }
    if (qt_key == Qt::Key_Control /*CTRL*/) {
        m_scene->position -= m_scene->up*0.1f;        }

    if (qt_key == Qt::Key_Plus) {
        m_scene->userDisplacementFactor += 0.005f;
    }
    else if (qt_key == Qt::Key_Minus) {
        m_scene->userDisplacementFactor -= 0.005f;
    }

    if (qt_key == Qt::Key_W) {
        m_scene->wireframe = !(m_scene->wireframe);
    }

    if (qt_key == Qt::Key_0) {
        m_scene->fboTexId = 0;      }
    else if (qt_key == Qt::Key_1) {
        m_scene->fboTexId = 1;      }
    else if (qt_key == Qt::Key_2) {
        m_scene->fboTexId = 2;      }
    else if (qt_key == Qt::Key_3) {
        m_scene->fboTexId = 3;      }
    else if (qt_key == Qt::Key_4) {
        m_scene->fboTexId = 4;      }
    else if (qt_key == Qt::Key_5) {
        m_scene->fboTexId = 5;      }
    else if (qt_key == Qt::Key_6) {
        m_scene->fboTexId = 6;      }
}

void Renderer::reloadShader()
{
    // Reload Shaders
    m_scene->s.renew();

    m_scene->s.addVertexShader("simple.vert");
    m_scene->s.addFragmentShader("simple.frag");
    m_scene->s.addTessControlShader("simple_tesc.glsl");
    m_scene->s.addTessEvaluationShader("simple_tese.glsl");
    m_scene->s.addGeometryShader("simple.geom");
    m_scene->s.link();
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
