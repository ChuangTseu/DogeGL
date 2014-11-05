#include "renderloop.h"

#include <chrono>
#include "GL.h"
#include "input.h"
#include "orthobase.h"
#include "texture.h"
#include "light.h"
#include "shader.h"
#include "camera.h"
#include "scene.h"
#include "fbo.h"

void RenderLoop(Scene& scene)
{
    FBO fbo;


    /* SHADERS */
    Shader s;

    s.addVertexShader("simple.vert");
    s.addFragmentShader("simple.frag");
    s.addTessControlShader("simple_tesc.glsl");
    s.addTessEvaluationShader("simple_tese.glsl");
    s.addGeometryShader("simple.geom");
    s.link();

    Shader quadFboShader;

//    quadFboShader.addVertexShader("quadFbo.vert");
    quadFboShader.addFragmentShader("quadFbo.frag");
    quadFboShader.link();


    /* CAMERA */

    Camera camera;

    camera.setProperties({0.f, 0.f, -1.f}, {0.f, 0.f, 1.f}, {0.f, 1.f, 0.f});

    mat4 projection = mat4::Projection(70, (float) scene.getWindowWidth()/scene.getWindowHeight(), 0.1f, 10000.f);

    vec3 up{0, 1, 0};
    vec3 position{5.f, 5.f, 5.f};
    vec3 forward = normalize(vec3{0, 0, 0} - position);


    /* INPUT */

    Input input;


    /* MODELS */

    OrthoBase base;

    Model cube;
//    cube.loadFromFile("cube.obj");

    Model plan;
//    plan.loadBasicType(Model::BasicType::PLAN);
    plan.loadFromFile("plan.obj");
//    plan.loadFromFile("arrow_cone.obj");

    Model basicLamp;
    basicLamp.loadFromFile("sphere.obj");
    Shader basicLampShader;
    basicLampShader.addVertexShader("line.vert");
    basicLampShader.addFragmentShader("line.frag");
    basicLampShader.link();

    Model quadFbo;
    quadFbo.loadFullscreenQuad();


    /* TEXTURES */

    Texture texture;
//    texture.loadFromFile("roundstones.jpg"); // EXAMPLE
    texture.loadFromFile("disp_data/wall002_512x512.jpg");

    Texture normalMap;
//    normalMap.loadFromFile("roundstones_norm.jpg");// EXAMPLE
    normalMap.loadFromFile("disp_data/wall002_nmap2_512x512.jpg");

    /* WATEEEEEEER */
//    Texture waterMaps[250];

//    std::string baseName("water_0055/water_055_c_");
//    std::string ext(".jpg");
//    std::string zerosStr[4]{"", "0", "00", "000"};

//    for (int i = 0; i < 250; ++i) {
//        std::string indexStr = std::to_string(i + 1);
//        std::string filename = baseName + zerosStr[4 - indexStr.length()] + indexStr + ext;

//        std::cerr << filename << '\n';
//        waterMaps[i].loadFromFile(filename);
//    }

//    int waterTexIndex = 0;

    Texture dogeMap;
//    dogeMap.loadFromFile("basic_displacement_map.png");
    dogeMap.loadFromFile("disp_data/wall002_hmap2_512x512.jpg");


    // Pure FPS Mode
    SDL_SetRelativeMouseMode(SDL_TRUE);


    /* VARIABLES */

    uint64_t counter = 0;
    uint64_t refreshCounterEvery = 3;

    float theta = 0;

    float userDisplacementFactor = 0.f;

    bool wireframe = false;


    /* DA MAIN LOOP*/

    while(!input.updateEvents() == Input::QUIT_EVENT)
    {
        const uint64_t start_time = SDL_GetPerformanceCounter();

        //Bed

        theta += 0.01f;
//        A += 0.03;

        float x = cosf(theta);
        float z = -sinf(theta);

        //position = {A*x, 5.f*sin(theta*3.f), A*z};

        /* CURRENT UGLY INPUT HANDLING */

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
            s.addGeometryShader("simple.geom");
            s.link();
        }

        if (input.getKey(SDL_SCANCODE_KP_PLUS)) {
            userDisplacementFactor += 0.005f;
        }
        else if (input.getKey(SDL_SCANCODE_KP_MINUS)) {
            userDisplacementFactor -= 0.005f;
        }

        if (input.getKey(SDL_SCANCODE_Z)) {
            wireframe = !wireframe;
        }

        /* AT LAST: DA RENDERING */

#define USE_FBO 1
#ifdef USE_FBO
        fbo.bind();

        GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1,  attachments);
#endif

//        glClearColor(1.f, 1.f, 1.f, 0); // WHITE
        glClearColor(0.f, 0.f, 0.f, 0); // BLACK
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//#define DISABLE_MAIN_RENDERING 1
#ifndef DISABLE_MAIN_RENDERING

        /* Bind a shader */
        s.use();

        /* Send data to the shader */
        glUniform1i(glGetUniformLocation(s.getProgramId(), "texSampler"), 0); //Texture unit 0 is for base images.
        glUniform1i(glGetUniformLocation(s.getProgramId(), "dispMapSampler"), 1); //...
        glUniform1i(glGetUniformLocation(s.getProgramId(), "normalMapSampler"), 2);
        texture.bindToTarget(GL_TEXTURE0);
        dogeMap.bindToTarget(GL_TEXTURE1);
        normalMap.bindToTarget(GL_TEXTURE2);

        /* WATEEEEEEER */
//        waterMaps[waterTexIndex].bindToTarget(GL_TEXTURE2);

//        if (counter == 0) {
//            waterTexIndex = (waterTexIndex + 1) % 250;
//        }

//        DirLight dirLight{vec3{1, 1, 1}, vec3{-1, 0, z}};

        DirLight dirLight{vec3{1, 1, 1}, vec3{-1, 0, 0}};
        PointLight pointLight{vec3{0, 1, 0}, vec3{x*10, 0, z*10}};

        glUniform3fv(glGetUniformLocation(s.getProgramId(), "dirLight.direction"), 1, dirLight.m_direction.data());
        glUniform3fv(glGetUniformLocation(s.getProgramId(), "dirLight.color"), 1, dirLight.m_color.data());

        glUniform3fv(glGetUniformLocation(s.getProgramId(), "pointLight.position"), 1, pointLight.m_position.data());
        glUniform3fv(glGetUniformLocation(s.getProgramId(), "pointLight.color"), 1, pointLight.m_color.data());

        glUniform3fv(glGetUniformLocation(s.getProgramId(), "eyePosition"), 1, position.data());

        /* Do your model transformations */
        mat4 cubeTransformation = mat4::Identity();
//        cubeTransformation.translate(5,0,0);
//        cubeTransformation.rotate(normalize({0.f, 0.f, 1.f}), -90.f);

        s.sendTransformations(projection, camera.getView(), cubeTransformation);

        glUniform1f(glGetUniformLocation(s.getProgramId(), "userDisplacementFactor"), userDisplacementFactor);
        glUniform1i(glGetUniformLocation(s.getProgramId(), "wireframe"), wireframe);

        /* FIRE! */
        plan.drawAsPatch();


        glUseProgram(0);


        mat4 lampTransformation = mat4::Identity();

        lampTransformation.translate(pointLight.m_position);

        basicLampShader.use();
        basicLampShader.sendTransformations(projection, camera.getView(), lampTransformation);
        glUniform1i(glGetUniformLocation(basicLampShader.getProgramId(), "overrideColor"), true);
        glUniform3fv(glGetUniformLocation(basicLampShader.getProgramId(), "userColor"), 1, pointLight.m_color.data());

        basicLamp.drawAsTriangles();

        glUseProgram(0);

        /* And so on for every other model... */
        /* Currently handmade, for developing/testing purposes */
        /* Will hopefully become moar professional in the near future */


        base.draw(projection, camera.getView());

#endif

#ifdef USE_FBO
        FBO::unbind();

        glClearColor(0.f, 0.f, 0.f, 0); // BLACK
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        quadFboShader.use();

        fbo.getTexture().bindToTarget(GL_TEXTURE0);

        quadFbo.drawAsFullscreenQuad();

        glUseProgram(0);
#endif


        /* SWAP BUFFERS */
        SDL_GL_SwapWindow(scene.getSDL_Window());

        /* ARBITRARILY LIMIT THE FRAMERATE, WE DON'T NEED OUR GPU TO CRY RENDERING A DUMB CUBE */
        SDL_Delay(8);


        /* HERE GOES OUR MAGNIFICENT PERFORMANCE MONITOR */
        uint64_t time_in_pc = SDL_GetPerformanceCounter() - start_time;
        double time_in_micros = time_in_pc / (SDL_GetPerformanceFrequency() / 1000000.0);

        char title_buffer[512];

        if (counter == refreshCounterEvery) {
            sprintf(title_buffer, "%s (%f us, ~%f FPS)", scene.getWindowTitle().c_str(), time_in_micros, 1000000/time_in_micros);
            SDL_SetWindowTitle(scene.getSDL_Window(), title_buffer);

            counter = 0;
        }
        else {
            ++counter;
        }
    }
}
