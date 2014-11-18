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
#include "cubemap.h"
#include "skybox.h"
#include "shadowmap.h"

void RenderLoop(Scene& scene)
{
    FBO fbo(scene.getWindowWidth(), scene.getWindowHeight(), 3);

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

    Mesh quadFbo;
    quadFbo.loadFullscreenQuad();


    /* CAMERA */

    Camera camera;

    camera.setProperties({0.f, 0.f, -1.f}, {0.f, 0.f, 1.f}, {0.f, 1.f, 0.f});

    mat4 projection = mat4::Projection(70, (float) scene.getWindowWidth()/scene.getWindowHeight(), 0.1f, 1000.f);

    vec3 up{0, 1, 0};
    vec3 position{5.f, 5.f, 5.f};
    vec3 forward = normalize(vec3{0, 0, 0} - position);


    /* INPUT */

    Input input;


    /* MODELS */

    Model plan;
//    plan.loadBasicType(Model::BasicType::PLAN);
//    plan.loadFromFile("plan.obj");
//    plan.loadFromFile("cube_and_floor.obj");
//    plan.loadFromFile("Worn_Down_House/destroyed_house.obj");
    plan.loadFromFile("hi_sphere.obj");

    Model basicLamp;
    basicLamp.loadFromFile("hi_sphere.obj");
    Shader basicLampShader;
    basicLampShader.addVertexShader("line.vert");
    basicLampShader.addFragmentShader("line.frag");
    basicLampShader.link();


    OrthoBase base;

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


    Cubemap cubemap;

    cubemap.loadFaceFromFile(Cubemap::Face::POSITIVE_X, "Ryfjallet_512_y_flipped/posx.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::NEGATIVE_X, "Ryfjallet_512_y_flipped/negx.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::POSITIVE_Y, "Ryfjallet_512_y_flipped/posy.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::NEGATIVE_Y, "Ryfjallet_512_y_flipped/negy.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::POSITIVE_Z, "Ryfjallet_512_y_flipped/posz.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::NEGATIVE_Z, "Ryfjallet_512_y_flipped/negz.jpg");


    Skybox skybox;

    skybox.feedCubemap(cubemap);


    /* SHADOW MAP */
    Shadowmap shadowmap(1024, 1024);



    // Pure FPS Mode
    SDL_SetRelativeMouseMode(SDL_TRUE);


    /* VARIABLES */

    uint64_t counter = 0;
    uint64_t refreshCounterEvery = 3;

    float theta = 0;

    float userDisplacementFactor = 0.f;

    bool wireframe = false;

    int fboTexId = 0;


    /* DA MAIN LOOP*/

    while(!input.updateEvents() == Input::QUIT_EVENT)
    {
//        glEnable(44);

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

        if (input.getKey(SDL_SCANCODE_KP_0)) {
            fboTexId = 0;      }
        else if (input.getKey(SDL_SCANCODE_KP_1)) {
            fboTexId = 1;      }
        else if (input.getKey(SDL_SCANCODE_KP_2)) {
            fboTexId = 2;      }
        else if (input.getKey(SDL_SCANCODE_KP_3)) {
            fboTexId = 3;      }
        else if (input.getKey(SDL_SCANCODE_KP_4)) {
            fboTexId = 4;      }
        else if (input.getKey(SDL_SCANCODE_KP_5)) {
            fboTexId = 5;      }
        else if (input.getKey(SDL_SCANCODE_KP_6)) {
            fboTexId = 6;      }



        /* AT LAST: DA RENDERING */


        /* Do your model transformations */
        mat4 cubeTransformation = mat4::Identity();
//        cubeTransformation.translate(5,0,0);
//        cubeTransformation.rotate(normalize({0.f, 0.f, 1.f}), -90.f);

        /* Get your lights ready */
        DirLight dirLight{vec3{1, 1, 1}, vec3{0, -1, -1}};
        PointLight pointLight{vec3{0, 1, 0}, vec3{x*10, 0, z*10}};


        fbo.bind();

        GLuint attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3,  attachments);


        glClearColor(0.f, 0.f, 0.f, 0); // BLACK
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        skybox.render(projection, camera.getPureViewNoTranslation());
//        skybox.render(projection, camera.getView());

        FBO::unbind();


        /* Bind a shader */
        s.use();

        /* Send data to the shader */
        glUniform1i(glGetUniformLocation(s.getProgramId(), "texSampler"), 0); //Texture unit 0 is for base images.
        glUniform1i(glGetUniformLocation(s.getProgramId(), "dispMapSampler"), 1); //...
        glUniform1i(glGetUniformLocation(s.getProgramId(), "normalMapSampler"), 2);
        glUniform1i(glGetUniformLocation(s.getProgramId(), "shadowMapSampler"), 3);
        texture.bindToTarget(GL_TEXTURE0);
        dogeMap.bindToTarget(GL_TEXTURE1);
        normalMap.bindToTarget(GL_TEXTURE2);

        glUniform1i(glGetUniformLocation(s.getProgramId(), "cubeMapSampler"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.getId());


        glUniform3fv(glGetUniformLocation(s.getProgramId(), "dirLight.direction"), 1, dirLight.m_direction.data());
        glUniform3fv(glGetUniformLocation(s.getProgramId(), "dirLight.color"), 1, dirLight.m_color.data());

        glUniform3fv(glGetUniformLocation(s.getProgramId(), "pointLight.position"), 1, pointLight.m_position.data());
        glUniform3fv(glGetUniformLocation(s.getProgramId(), "pointLight.color"), 1, pointLight.m_color.data());

        glUniform3fv(glGetUniformLocation(s.getProgramId(), "eyePosition"), 1, position.data());


        glUniform1f(glGetUniformLocation(s.getProgramId(), "userDisplacementFactor"), userDisplacementFactor);
        glUniform1i(glGetUniformLocation(s.getProgramId(), "wireframe"), wireframe);



        /* SHADOW PASS */
        mat4 lightProjection = mat4::Projection(70, 1.f, 0.1f, 1000.f);
        mat4 lightView = mat4::LookAt(vec3{15.f, 15.f, 15.f}, vec3{0, 0, 0}, vec3{0, 1, 0});

        mat4 lightMVP = lightProjection * lightView;        

        s.sendTransformations(lightProjection, lightView, cubeTransformation);

        shadowmap.bind();

        plan.drawAsPatch();

        shadowmap.unbind();

        /* FIRE! */
        s.sendTransformations(projection, camera.getView(), cubeTransformation);
        glUniformMatrix4fv(glGetUniformLocation(s.getProgramId(), "lightMVP"), 1, GL_FALSE, lightMVP.data());

        shadowmap.bindShadowMapToTarget(GL_TEXTURE3);

        fbo.bind();

        plan.drawAsPatch(&s);

        Shader::unbind();


        mat4 lampTransformation = mat4::Identity();

        lampTransformation.translate(pointLight.m_position);

        basicLampShader.use();
        basicLampShader.sendTransformations(projection, camera.getView(), lampTransformation);
        glUniform1i(glGetUniformLocation(basicLampShader.getProgramId(), "overrideColor"), true);
        glUniform3fv(glGetUniformLocation(basicLampShader.getProgramId(), "userColor"), 1, pointLight.m_color.data());

        basicLamp.drawAsTriangles();

        Shader::unbind();

        /* And so on for every other model... */
        /* Currently handmade, for developing/testing purposes */
        /* Will hopefully become moar professional in the near future */


        base.draw(projection, camera.getView());

        FBO::unbind();

        glClearColor(0.f, 0.f, 0.f, 0); // BLACK
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        quadFboShader.use();
        glUniform1i(glGetUniformLocation(quadFboShader.getProgramId(), "screenWidth"), scene.getWindowWidth());
        glUniform1i(glGetUniformLocation(quadFboShader.getProgramId(), "screenHeight"), scene.getWindowHeight());

        fbo.getTexture(fboTexId).bindToTarget(GL_TEXTURE0);
//        shadowFbo.getTexture(0).bindToTarget(GL_TEXTURE0);

        quadFbo.draw();

        Shader::unbind();


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
