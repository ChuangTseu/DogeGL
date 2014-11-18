#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "material.h"
#include "shader.h"

#include <string>
#include <vector>

class Model
{
    std::vector<Mesh> m_meshes;
    std::vector<Material> m_materials;

public:
    enum class BasicType {
        CUBE, PYRAMID, TRIANGLE, PLAN
    };

public:
    Model();

//    bool loadBasicType(BasicType type);

    bool loadFromFile(const std::string& filename); // Later, loadFrom


    bool loadBasicType(BasicType type);
    bool loadFullscreenQuad();

    void draw(Shader* s = nullptr) const;
    void drawAsTriangles(Shader* s = nullptr) const;
    void drawAsPatch(Shader* s = nullptr) const;
};

#endif // MODEL_H
