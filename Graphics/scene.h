#ifndef SCENE_H
#define SCENE_H

#include "orthobase.h"
#include "texture.h"
#include "light.h"
#include "shader.h"
#include "camera.h"
#include "fbo.h"
#include "cubemap.h"
#include "skybox.h"
#include "shadowmap.h"

#include "MathsTools/mat4.h"
#include "MathsTools/vec3.h"

class Scene
{
public:
    Scene(int width, int height);

    void initScene();

    void render();

public:
    int m_width;
    int m_height;

    FBO fbo{m_width, m_height, 3};

    /* SHADERS */
    Shader s;

    Shader quadFboShader;

    Mesh quadFbo;

    Camera camera;

    mat4 projection = mat4::Projection(70, (float) m_width/m_height, 0.1f, 1000.f);

    vec3 up = vec3{0, 1, 0};
    vec3 position = vec3{5.f, 5.f, 5.f};
    vec3 forward = normalize(vec3{0, 0, 0} - position);

    Model mainModel;


    Model basicLamp;
    Shader basicLampShader;


    OrthoBase base;

    Texture texture;
    Texture normalMap;
    Texture dogeMap;

    Cubemap cubemap;

    Skybox skybox;

    Shadowmap shadowmap{1024, 1024};

    float theta = 0;

    float userDisplacementFactor = 0.f;

    bool wireframe = false;

    int fboTexId = 0;
};

#endif // SCENETORENAME_H
