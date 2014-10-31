#ifndef MODEL_H
#define MODEL_H

#include "GL.h"
#include <vector>
#include <iostream>

#include <assimp/postprocess.h>

#include "vbo.h"
#include "ibo.h"

#include "vertex.h"

class Model
{
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;

    VBO m_vbo;
    IBO m_ibo;

    GLuint m_vao;

    bool m_hasNormal;

public:
    enum class BasicType {
        CUBE, PYRAMID, TRIANGLE, PLAN
    };

public:
    Model();

//    bool loadBasicType(BasicType type);

    bool loadFromFile(const std::string& filename); // Later, loadFrom

    void draw();
    bool loadBasicType(BasicType type);
};

#endif // MODEL_H
