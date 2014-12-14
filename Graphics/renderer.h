#ifndef RENDERER_H
#define RENDERER_H

#include "QtGUI/rendererinterface.h"

#include "scene.h"

#include <string>

class Renderer : public RendererInterface
{
public:
    Renderer(int width, int height);

    void initializeGL(void);
    void resizeGL(int width, int height);
    void paintGL(void);

    void loadModel(const std::string &filename);
    void loadEnvironmentMap(const std::string& filename);

    void onKeyPress(int qt_key);

    void reloadShader();
    void toggleWireframe();
    void setFinalFboTarget(int targetIndex);

    bool initGL();
    void initScene();

    void render();

    void rotateCamera(int mouse_x_rel, int mouse_y_rel);
    void translateCamera(int mouse_x_rel, int mouse_y_rel, int mouse_z_rel);

    void gammaChanged(float value);
    void keyValueChanged(float value);

private:
    int m_width;
    int m_height;

//CURRENT RENDERLOOP DATA: TO REFACTOR !!!!
private:
    Scene* m_scene{nullptr};
};

#endif // RENDERER_H
