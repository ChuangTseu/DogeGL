#include "scene.h"

#include "Utils/timer.h"

Scene::Scene(int width, int height) :
    m_width(width),
    m_height(height),
    reducePass(m_width, m_height, "reduce.frag"),
    finalScreenPass(m_width, m_height, "quadFbo.frag"),
    toLuminancePass(m_width, m_height, "toLuminance.frag"),
    tonemapPass(m_width, m_height, "tonemap.frag")
{
}

void Scene::initScene() {
    s.addVertexShader("simple.vert");
    s.addFragmentShader("simple.frag");
    s.addTessControlShader("simple_tesc.glsl");
    s.addTessEvaluationShader("simple_tese.glsl");
    s.addGeometryShader("simple.geom");
    s.link();


    renewableShadersList.push_back(&(s));
    renewableShadersList.push_back(&(reducePass.getShader()));
    renewableShadersList.push_back(&(finalScreenPass.getShader()));
    renewableShadersList.push_back(&(toLuminancePass.getShader()));
    renewableShadersList.push_back(&(tonemapPass.getShader()));
    renewableShadersList.push_back(&(skybox.getShader()));


//    quadFboShader.addFragmentShader("quadFbo.frag");
//    quadFboShader.link();

//    reduceShader.addFragmentShader("reduce.frag");
//    reduceShader.link();

//    quadFbo.loadFullscreenQuad();


    camera.setProperties(vec3{0.f, 0.f, -1.f}, vec3{0.f, 0.f, 1.f}, vec3{0.f, 1.f, 0.f});

//    mainModel.loadBasicType(Model::BasicType::PLAN);
//    mainModel.loadFromFile("plan.obj");
//    mainModel.loadFromFile("cube_and_floor.obj");
//    mainModel.loadFromFile("Worn_Down_House/destroyed_house.obj");
//    mainModel.loadFromFile("hi_sphere.obj");
//    mainModel.loadFromFile("Astroboy/astroBoy_walk_Maya.dae");
    mainModel.loadFromFile("SimpleModel/demo.dae");
//    mainModel.loadFromFile("cubenorm.obj");

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


    cubemap.loadFaceFromFile(Cubemap::Face::POSITIVE_X, "Ryfjallet_512/posx.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::NEGATIVE_X, "Ryfjallet_512/negx.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::POSITIVE_Y, "Ryfjallet_512/posy.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::NEGATIVE_Y, "Ryfjallet_512/negy.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::POSITIVE_Z, "Ryfjallet_512/posz.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::NEGATIVE_Z, "Ryfjallet_512/negz.jpg");

//    cubemap.loadFaceFromFile(Cubemap::Face::POSITIVE_X, "grace/posx.jpg");
//    cubemap.loadFaceFromFile(Cubemap::Face::NEGATIVE_X, "grace/negx.jpg");
//    cubemap.loadFaceFromFile(Cubemap::Face::POSITIVE_Y, "grace/posy.jpg");
//    cubemap.loadFaceFromFile(Cubemap::Face::NEGATIVE_Y, "grace/negy.jpg");
//    cubemap.loadFaceFromFile(Cubemap::Face::POSITIVE_Z, "grace/posz.jpg");
//    cubemap.loadFaceFromFile(Cubemap::Face::NEGATIVE_Z, "grace/negz.jpg");

    skybox.feedCubemap(cubemap);

    fbo.reset(new FBO(m_width, m_height, 3));
}

void Scene::resize(int width, int height)
{
    m_width = width;
    m_height = height;

    projection = mat4::Projection(70, (float) m_width / (float) m_height, 0.1f, 1000.f);

    fbo.reset(new FBO(width, height, 3));

    finalScreenPass.resize(width, height);

    toLuminancePass.resize(width, height);

    tonemapPass.resize(width, height);

    std::cerr << "FBO shall be: " << width << " " << height << '\n';
}

void Scene::reloadShaders()
{
    for (Shader* pShader : renewableShadersList) {
        pShader->reload();
    }
}

static bool isPowerOfTwo(int val) {
    return (val > 0) && !(val & (val - 1));
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
    DirLight dirLight{vec3{2, 2, 2}, vec3{0, -1, -1}};
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


    unsigned int closestWidth = m_width;
    unsigned int closestHeight = m_height;

    if (!isPowerOfTwo(closestWidth)) {
        float f = (float)(closestWidth - 1);
        closestWidth = 1U << ((*(unsigned int*)(&f) >> 23) - 126 - 1);
    }
    if (!isPowerOfTwo(closestHeight)) {
        float f = (float)(closestHeight - 1);
        closestHeight = 1U << ((*(unsigned int*)(&f) >> 23) - 126 - 1);
    }

    SingleColorFBO toLuminanceFbo(closestWidth, closestHeight);

    toLuminanceFbo.bind();

    glClear(GL_DEPTH_BUFFER_BIT);

    fbo->getTexture(0).bindToTarget(GL_TEXTURE0);
    toLuminancePass.resize(closestWidth, closestHeight);

    toLuminancePass.fire();

    toLuminanceFbo.bindToTarget(GL_TEXTURE0);

    std::cerr << "Closest width x height " << closestWidth << " x " << closestHeight << '\n';

//    exit(0);


    //First try -> do reduce only on power of two textures, should be easy mode
    if (true /*(m_width == m_height) && isPowerOfTwo(m_width)*/) {
//        Timer timer;

        toLuminanceFbo.bindToTarget(GL_TEXTURE0);

//        GLint param;
//        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &param);
//        std::cerr << param << '\n';
//        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &param);
//        std::cerr << param << '\n';
//        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &param);
//        std::cerr << param << '\n';
//        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_PACK_ALIGNMENT, &param);
//        std::cerr << param << '\n';

////        timer.glStart();

//        float* fPixel = new float[m_height*m_width*4];

//        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, fPixel);

//        float mean_x = 0;
//        float mean_y = 0;
//        float mean_z = 0;
//        float logAverageLum = 0;

//        for (int i = 0; i < closestHeight; ++i) {
//            for (int j = 0; j < closestWidth; ++j) {
//                mean_x += fPixel[i*closestWidth*4 + j*4 + 0];
//                mean_y += fPixel[i*closestWidth*4 + j*4 + 1];
//                mean_z += fPixel[i*closestWidth*4 + j*4 + 2];
//                logAverageLum += fPixel[i*closestWidth*4 + j*4 + 3];
//            }
//        }

//        mean_x /= (float) closestWidth*closestHeight;
//        mean_y /= (float) closestWidth*closestHeight;
//        mean_z /= (float) closestWidth*closestHeight;
//        logAverageLum /= (float) closestWidth*closestHeight;

//        std::cerr << "Mean RGB = " << mean_x << ' ' << mean_y << ' ' << mean_z << ' ' <<  logAverageLum << '\n';

//        delete fPixel;

//        timer.glStop();
//        std::cerr << timer.getElapsedTimeInMilliSec() << " ms.\n";

        int n = 0;

        int numLevels = 1 + floor(log2(std::max(closestWidth, closestHeight)));

//        std::vector<SingleColorFBO> tricheurs;

//        tricheurs.reserve(numLevels - 1);

//        while ((m_width >> (n + 1)) > 0 && (m_height >> (n + 1)) > 0) {
//            tricheurs.emplace_back(m_width >> (n + 1), m_height >> (n + 1), GL_NEAREST);

//            ++n;
//        }

//        timer.glStart();

//        glDisable(GL_DEPTH_TEST);

//        tricheurs[0].bind();

//        reducePass.resize(m_width >> (0 + 1), m_height >> (0 + 1));
//        fbo->getTexture(0).bindToTarget(GL_TEXTURE0);

//        reducePass.fire();


//        for (int i = 1; i < n; ++i) {
//            tricheurs[i].bind();

//            tricheurs[i - 1].bindToTarget(GL_TEXTURE0);

//            reducePass.resize(m_width >> (i + 1), m_height >> (i + 1));

//            reducePass.fire();
//        }

//        timer.glStop();
//        std::cerr << timer.getElapsedTimeInMilliSec() << " ms.\n";

//        tricheurs[8].bindToTarget(GL_TEXTURE0);

//        GLint param;
//        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &param);
//        std::cerr << param << '\n';
//        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &param);
//        std::cerr << param << '\n';
//        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &param);
//        std::cerr << param << '\n';
//        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_PACK_ALIGNMENT, &param);
//        std::cerr << param << '\n';

        float onePixel[4] = {0};
//        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, onePixel);

//        std::cerr << "Mean RGB - " << onePixel[0] << ' ' << onePixel[1] << ' ' << onePixel[2] << '\n';

//        glEnable(GL_DEPTH_TEST);

//        timer.glStop();
//        std::cerr << timer.getElapsedTimeInMilliSec() << " ms.\n";

//        tricheurs[7].bindToTarget(GL_TEXTURE0);

//        float fourPixel[4][4] = {0};
//        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, fourPixel);

//        std::cerr << "           " << fourPixel[0][0] << ' ' << fourPixel[0][1] << ' ' << fourPixel[0][2] << '\t';
//        std::cerr << "           " << fourPixel[1][0] << ' ' << fourPixel[1][1] << ' ' << fourPixel[1][2] << '\n';
//        std::cerr << "           " << fourPixel[2][0] << ' ' << fourPixel[2][1] << ' ' << fourPixel[2][2] << '\t';
//        std::cerr << "           " << fourPixel[3][0] << ' ' << fourPixel[3][1] << ' ' << fourPixel[3][2] << '\n';

//        timer.glStart();

        GLuint m_query;
        GLuint elapsed_time = 0;

        glGenQueries(1, &m_query);

        glBeginQuery(GL_TIME_ELAPSED, m_query);

        toLuminanceFbo.bindToTarget(GL_TEXTURE0);

        glGenerateMipmap(GL_TEXTURE_2D);

        glEndQuery(GL_TIME_ELAPSED);

        GLint done = 0;
        while (!done)
        {
            glGetQueryObjectiv(m_query, GL_QUERY_RESULT_AVAILABLE, &done);
        }
        glGetQueryObjectuiv(m_query, GL_QUERY_RESULT, &elapsed_time);

//        std::cerr << elapsed_time / 1000000.0 << " ms.\n";

//        timer.glStop();
//        std::cerr << timer.getElapsedTimeInMilliSec() << " ms.\n";

        onePixel[0] = 0;
        onePixel[1] = 0;
        onePixel[2] = 0;
        onePixel[3] = 0;

        glGetTexImage(GL_TEXTURE_2D, numLevels - 1, GL_RGBA, GL_FLOAT, onePixel);

        std::cerr << "Mean RGB - " << onePixel[0] << ' ' << onePixel[1] << ' ' << onePixel[2] << ' ' << onePixel[3] << '\n';

//        timer.glStop();
//        std::cerr << timer.getElapsedTimeInMilliSec() << " ms.\n";

//        exit(0);

        FBO::unbind();

//        tricheurs[0].bindToTarget(GL_TEXTURE0);
        fbo->getTexture(fboTexId).bindToTarget(GL_TEXTURE0);
//        toLuminanceFbo.bindToTarget(GL_TEXTURE0);

        Shader& tonemapShader = tonemapPass.getShader();
        tonemapShader.use();

        glUniform1f(glGetUniformLocation(tonemapShader.getProgramId(), "logAvLum"), onePixel[3]);
        glUniform1f(glGetUniformLocation(tonemapShader.getProgramId(), "gamma"), gamma);
        glUniform1f(glGetUniformLocation(tonemapShader.getProgramId(), "keyValue"), keyValue);
        tonemapPass.fire();

//        finalScreenPass.fire();
    }
    else {
        FBO::unbind();

        fbo->getTexture(fboTexId).bindToTarget(GL_TEXTURE0);
    //        shadowFbo.getTexture(0).bindToTarget(GL_TEXTURE0);
        finalScreenPass.fire();
    }


    Shader::unbind();
}
