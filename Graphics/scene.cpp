#include "scene.h"

Scene::Scene(int width, int height) :
    m_width(width),
    m_height(height)
{
}

void Scene::initScene() {
    s.addVertexShader("simple.vert");
    s.addFragmentShader("simple.frag");
    s.addTessControlShader("simple_tesc.glsl");
    s.addTessEvaluationShader("simple_tese.glsl");
    s.addGeometryShader("simple.geom");
    s.link();


    quadFboShader.addFragmentShader("quadFbo.frag");
    quadFboShader.link();

    quadFbo.loadFullscreenQuad();


    camera.setProperties(vec3{0.f, 0.f, -1.f}, vec3{0.f, 0.f, 1.f}, vec3{0.f, 1.f, 0.f});

//    mainModel.loadBasicType(Model::BasicType::PLAN);
//    mainModel.loadFromFile("plan.obj");
//    mainModel.loadFromFile("cube_and_floor.obj");
//    mainModel.loadFromFile("Worn_Down_House/destroyed_house.obj");
//    mainModel.loadFromFile("hi_sphere.obj");
//    mainModel.loadFromFile("Astroboy/astroBoy_walk_Maya.dae");
//    mainModel.loadFromFile("SimpleModel/demo.dae");
    mainModel.loadFromFile("cubenorm.obj");

    basicLamp.loadFromFile("hi_sphere.obj");

    basicLampShader.addVertexShader("line.vert");
    basicLampShader.addFragmentShader("line.frag");
    basicLampShader.link();


//    texture.loadFromFile("roundstones.jpg"); // EXAMPLE
    texture.loadFromFile("disp_data/wall002_512x512.jpg");

//    normalMap.loadFromFile("roundstones_norm.jpg");// EXAMPLE
    normalMap.loadFromFile("disp_data/wall002_nmap2_512x512.jpg");

//    dogeMap.loadFromFile("basic_displacement_map.png");
    dogeMap.loadFromFile("disp_data/wall002_hmap2_512x512.jpg");


    cubemap.loadFaceFromFile(Cubemap::Face::POSITIVE_X, "Ryfjallet_512_y_flipped/posx.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::NEGATIVE_X, "Ryfjallet_512_y_flipped/negx.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::POSITIVE_Y, "Ryfjallet_512_y_flipped/posy.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::NEGATIVE_Y, "Ryfjallet_512_y_flipped/negy.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::POSITIVE_Z, "Ryfjallet_512_y_flipped/posz.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::NEGATIVE_Z, "Ryfjallet_512_y_flipped/negz.jpg");

    skybox.feedCubemap(cubemap);

    fbo.reset(new FBO(m_width, m_height, 3));
}

void Scene::resize(int width, int height)
{
    m_width = width;
    m_height = height;

    projection = mat4::Projection(70, (float) m_width / (float) m_height, 0.1f, 1000.f);

    fbo.reset(new FBO(width, height, 3));
}



void Scene::render()
{
    //Bed

    theta += 0.01f;
    //        A += 0.03;

    float x = cosf(theta);
    float z = -sinf(theta);

    //position = {A*x, 5.f*sin(theta*3.f), A*z};


    forward.normalize();

    camera.lookAt(position, position + forward, up);

    /* AT LAST: DA RENDERING */


    /* Do your model transformations */
    mat4 cubeTransformation = mat4::Identity();
//        cubeTransformation.translate(5,0,0);
//        cubeTransformation.rotate(normalize({0.f, 0.f, 1.f}), -90.f);

    /* Get your lights ready */
    DirLight dirLight{vec3{1, 1, 1}, vec3{0, -1, -1}};
    PointLight pointLight{vec3{0, 1, 0}, vec3{x*10, 0, z*10}};


    fbo->bind();

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
    glUniform1i(glGetUniformLocation(s.getProgramId(), "normalMapSampler"), 1);
    glUniform1i(glGetUniformLocation(s.getProgramId(), "dispMapSampler"), 2); //...
    glUniform1i(glGetUniformLocation(s.getProgramId(), "shadowMapSampler"), 3);
//        texture.bindToTarget(GL_TEXTURE0);
    dogeMap.bindToTarget(GL_TEXTURE2);

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
    glPolygonOffset(1.1f, 4.0f);
    glEnable(GL_POLYGON_OFFSET_FILL);

    mat4 lightProjection = mat4::Projection(70, 1.f, 0.1f, 1000.f);
    mat4 lightView = mat4::LookAt(vec3{15.f, 15.f, 15.f}, vec3{0, 0, 0}, vec3{0, 1, 0});

    mat4 lightMVP = lightProjection * lightView;

    s.sendTransformations(lightProjection, lightView, cubeTransformation);

    shadowmap.bind();

    mainModel.drawAsPatch(lightProjection, lightView, cubeTransformation, &s);

    shadowmap.unbind();

    glDisable(GL_POLYGON_OFFSET_FILL);

    /* FIRE! */
    s.sendTransformations(projection, camera.getView(), cubeTransformation);
    glUniformMatrix4fv(glGetUniformLocation(s.getProgramId(), "lightMVP"), 1, GL_FALSE, lightMVP.data());

    shadowmap.bindShadowMapToTarget(GL_TEXTURE3);

    fbo->bind();

    mainModel.drawAsPatch(projection, camera.getView(), cubeTransformation, &s);

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
    glUniform1i(glGetUniformLocation(quadFboShader.getProgramId(), "screenWidth"), m_width);
    glUniform1i(glGetUniformLocation(quadFboShader.getProgramId(), "screenHeight"), m_height);

    fbo->getTexture(fboTexId).bindToTarget(GL_TEXTURE0);
//        shadowFbo.getTexture(0).bindToTarget(GL_TEXTURE0);

    quadFbo.draw();

    Shader::unbind();
}
