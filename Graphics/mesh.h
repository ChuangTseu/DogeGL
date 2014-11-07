#ifndef MESH_H
#define MESH_H

#include "GL.h"
#include <vector>
#include <iostream>

#include "vbo.h"
#include "ibo.h"

#include "vertex.h"

struct aiMesh;

struct Mesh {
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;

    VBO m_vbo;
    IBO m_ibo;

    GLuint m_vao;

    bool loadFromAssimpMesh(const aiMesh* mesh);

    void draw() const;

    void drawAsTriangles() const;

    void drawAsPatch() const;

    bool loadFullscreenQuad();
};

#endif // MESH_H
