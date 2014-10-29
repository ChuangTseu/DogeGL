#include "scene.h"

Scene::Scene()
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


    // Double Buffer

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Window

    m_window = SDL_CreateWindow(m_windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_windowWidth, m_windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

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

bool Scene::initGL()
{
    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "OpenGL Vendor : " << glGetString(GL_VENDOR) << std::endl;

    #ifdef WIN32

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


    #endif

    // Activation du Depth Buffer

    glEnable(GL_DEPTH_TEST);

    /*glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);*/

    return true;
}

#include <chrono>
#include "input.h"
#include "orthobase.h"
#include "texture.h"

void Scene::mainLoop()
{
    Shader s;

    s.addVertexShader("simple.vert");
    s.addFragmentShader("simple.frag");
    s.addTessControlShader("simple_tesc.glsl");
    s.addTessEvaluationShader("simple_tese.glsl");
    s.link();

    Shader lineShader;

    lineShader.addVertexShader("line.vert");
    lineShader.addFragmentShader("line.frag");
    lineShader.link();

    Camera camera;

    camera.setProperties({0.f, 0.f, -1.f}, {0.f, 0.f, 1.f}, {0.f, 1.f, 0.f});

    mat4 projection = mat4::Projection(70, (float) m_windowWidth/m_windowHeight, 0.1f, 100.f);

    vec3 up{0, 1, 0};    
    vec3 position{5.f, 5.f, 5.f};
    vec3 forward = normalize(vec3{0, 0, 0} - position);

    Input input;

    OrthoBase base;
    Model cube;

    cube.loadFromFile("cube.obj");

    Texture texture;
    texture.loadFromFile("doge_512.jpg");

    float theta = 0;

    // Pure FPS Mode
    SDL_SetRelativeMouseMode(SDL_TRUE);

    uint64_t counter = 0;
    uint64_t refreshCounterEvery = 10;


    while(!input.updateEvents() == Input::QUIT_EVENT)
    {
        const uint64_t start_time = SDL_GetPerformanceCounter();

        //Bed

        theta += 0.1f;
        //A += 0.03;

//        x = cosf(theta);
//        z = -sinf(theta);

        //position = {A*x, 5.f*sin(theta*3.f), A*z};

        // WARNING: Following content rated mature, keep children far from this terrible stuff
        int mouse_x_rel = input.getXRel();
        int mouse_y_rel = input.getYRel();

        vec3 kright = normalize(cross(forward, up));
        vec3 kdown = normalize(cross(forward, kright));

        forward += kright*(mouse_x_rel/100.f);
        forward += kdown*(mouse_y_rel/100.f);

        forward.normalize();

        if (input.getKey(SDL_SCANCODE_W)) {
            position += forward*0.1f;        }
        if (input.getKey(SDL_SCANCODE_S)) {
            position -= forward*0.1f;        }
        if (input.getKey(SDL_SCANCODE_A)) {
            position -= kright*0.1f;        }
        if (input.getKey(SDL_SCANCODE_D)) {
            position += kright*0.1f;        }
        if (input.getKey(SDL_SCANCODE_LSHIFT)) {
            position += up*0.1f;        }
        if (input.getKey(SDL_SCANCODE_LCTRL)) {
            position -= up*0.1f;        }

        camera.lookAt(position, position + forward, up);


        if (input.getKey(SDL_SCANCODE_R)) {
            // Reload Shaders
            s.renew();

            s.addVertexShader("simple.vert");
            s.addFragmentShader("simple.frag");
            s.addTessControlShader("simple_tesc.glsl");
            s.addTessEvaluationShader("simple_tese.glsl");
            s.link();
        }


        //EndBed

        glClearColor(1.f, 1.f, 1.f, 0); // WHITE
        //glClearColor(0.f, 0.f, 0.f, 0); // BLACK
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 cubeTransformation = mat4::Identity();

//        cubeTransformation.translate(0.f, 0.f, theta / 20.f);
        cubeTransformation.rotate(normalize({0.f, 1.f, 0.f}), theta);
//        cubeTransformation.scale(1 + theta / 20, 1, 1);

        s.use();
        s.sendTransformations(projection, camera.getView(), cubeTransformation);

        texture.bindToTarget(GL_TEXTURE0);

        cube.draw();

        glUseProgram(0);


        lineShader.use();
        lineShader.sendTransformations(projection, camera.getView(), mat4::Identity());

        base.draw();

        glUseProgram(0);


        SDL_GL_SwapWindow(m_window);
        SDL_Delay(8);

        uint64_t time_in_pc = SDL_GetPerformanceCounter() - start_time;
        double time_in_micros = time_in_pc / (SDL_GetPerformanceFrequency() / 1000000.0);

        char title_buffer[512];

        if (counter == refreshCounterEvery) {
            sprintf(title_buffer, "%s (%f us, ~%f FPS)", m_windowTitle.c_str(), time_in_micros, 1000000/time_in_micros);
            SDL_SetWindowTitle(m_window, title_buffer);

            counter = 0;
        }

        ++counter;
    }
}
