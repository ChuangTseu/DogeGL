#ifndef MESH_H
#define MESH_H

#include "GL.h"
#include <vector>
#include <iostream>

#include "vbo.h"
#include "ibo.h"
#include "vao.h"

#include "vertex.h"

struct aiMesh;

struct Mesh {
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;

    VBO m_vbo;
    IBO m_ibo;

    VAO m_vao;

    unsigned int m_materialIndex;

    bool loadFromAssimpMesh(const aiMesh* mesh);

    void draw();

    void drawAsTriangles();

    void drawAsPatch();

    bool loadFullscreenQuad();
};

#endif // MESH_H
