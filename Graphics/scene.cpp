#include "scene.h"

Scene::Scene(int width, int height, int type) : m_windowWidth(width),
    m_windowHeight(height),
    m_type(type)
{
}

bool Scene::initWindow() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }

    // OpenGL version

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
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

    m_window = SDL_CreateWindow(m_windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                m_windowWidth, m_windowHeight, windowAttributesFlags);

    if(!m_window)
    {
        std::cout << "SDL Error : " << SDL_GetError() << std::endl;
        return false;
    }

    //OpenGL context

    m_openGLContext = SDL_GL_CreateContext(m_window);

    if(m_openGLContext == 0)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();

        return false;
    }

    return true;
}

void GLAPIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                     const GLchar * message, const void * userParam) {
    (void) source;
    (void) type;
    (void) id;
    (void) severity;
    (void) length;
    (void) message;
    (void) userParam;

    std::cerr << "Error\n";
    return;
}

static void glBreak_print_source(GLenum source)
{
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
        {
            fputs("GL_DEBUG_SOURCE_API", stderr);
        }
        break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        {
            fputs("GL_DEBUG_SOURCE_WINDOW_SYSTEM", stderr);
        }
        break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
        {
            fputs("GL_DEBUG_SOURCE_SHADER_COMPILER", stderr);
        }
        break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
        {
            fputs("GL_DEBUG_SOURCE_THIRD_PARTY", stderr);
        }
        break;

        case GL_DEBUG_SOURCE_APPLICATION:
        {
            fputs("GL_DEBUG_SOURCE_APPLICATION", stderr);
        }
        break;

        case GL_DEBUG_SOURCE_OTHER:
        {
            fputs("GL_DEBUG_SOURCE_APPLICATION", stderr);
        }
        break;
    }
}

static void glBreak_print_type(GLenum type)
{
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
        {
            fputs("GL_DEBUG_TYPE_ERROR", stderr);
        }
        break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        {
            fputs("GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR", stderr);
        }
        break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        {
            fputs("GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR", stderr);
        }
        break;

        case GL_DEBUG_TYPE_PORTABILITY:
        {
            fputs("GL_DEBUG_TYPE_PORTABILITY", stderr);
        }
        break;

        case GL_DEBUG_TYPE_PERFORMANCE:
        {
            fputs("GL_DEBUG_TYPE_PERFORMANCE", stderr);
        }
        break;

        case GL_DEBUG_TYPE_MARKER:
        {
            fputs("GL_DEBUG_TYPE_MARKER", stderr);
        }
        break;

        case GL_DEBUG_TYPE_PUSH_GROUP:
        {
            fputs("GL_DEBUG_TYPE_PUSH_GROUP", stderr);
        }
        break;

        case GL_DEBUG_TYPE_POP_GROUP:
        {
            fputs("GL_DEBUG_TYPE_POP_GROUP", stderr);
        }
        break;

        case GL_DEBUG_TYPE_OTHER:
        {
            fputs("GL_DEBUG_TYPE_OTHER", stderr);
        }
        break;
    }
}

static void glBreak_print_severity(GLenum severity)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_LOW:
        {
            fputs("GL_DEBUG_SEVERITY_LOW", stderr);
        }
        break;

        case GL_DEBUG_SEVERITY_MEDIUM:
        {
            fputs("GL_DEBUG_SEVERITY_MEDIUM", stderr);
        }
        break;

        case GL_DEBUG_SEVERITY_HIGH:
        {
            fputs("GL_DEBUG_SEVERITY_HIGH", stderr);
        }
        break;
    }
}

/**
 * @brief glBreak_debug
 * @param source
 * @param type
 * @param id
 * @param severity
 * @param length
 * @param message
 * @param userParam
 */
void GLAPIENTRY glBreak_debug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam)
{
    (void) id;
    (void) length;
    (void) userParam;

    fputs("---- GL BREAKPOINT ----\n", stderr);

    fputs("Source : ", stderr);
    glBreak_print_source(source);

    fputs("\n", stderr);

    fputs("Type : ", stderr);
    glBreak_print_type(type);

    fputs("\n", stderr);

    fputs("Severity : ", stderr);
    glBreak_print_severity(severity);

    fputs("\n", stderr);

    fputs("Message : ", stderr);
    fputs(message, stderr);

    fputs("-----------------------\n", stderr);

    if (GL_DEBUG_TYPE_ERROR == type)
    {
        asm("int3");
    }
}

bool Scene::initGL()
{
    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "OpenGL Vendor : " << glGetString(GL_VENDOR) << std::endl;


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

        SDL_GL_DeleteContext(m_openGLContext);
        SDL_DestroyWindow(m_window);
        SDL_Quit();

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

#include "renderloop.h"

void Scene::mainLoop()
{
    RenderLoop(*this);
}
