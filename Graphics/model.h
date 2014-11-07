#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include <string>
#include <vector>

class Model
{
    std::vector<Mesh> m_meshes;

public:
    enum class BasicType {
        CUBE, PYRAMID, TRIANGLE, PLAN
    };

public:
    Model();

//    bool loadBasicType(BasicType type);

    bool loadFromFile(const std::string& filename); // Later, loadFrom

    void draw() const;
    bool loadBasicType(BasicType type);
    void drawAsTriangles() const;
    void drawAsPatch() const;
    bool loadFullscreenQuad();
};

#endif // MODEL_H
