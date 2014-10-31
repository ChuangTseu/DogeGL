#ifndef ORTHOBASE_H
#define ORTHOBASE_H

#include <vector>
#include <iostream>

#include <assimp/postprocess.h>

#include "GL.h"

#include "vbo.h"
#include "ibo.h"

#include "vertex.h"

class OrthoBase
{
    VBO m_vbo;
    GLuint m_vao;

    std::vector<Vertex> m_vertices;

public:
    OrthoBase() {
        vec3 origin{0, 0, 0};
        vec3 xaxis{1, 0, 0};
        vec3 yaxis{0, 1, 0};
        vec3 zaxis{0, 0, 1};

        vec2 uvcoord{0, 0};
        vec3 tangent{0, 0, 0};

        float axisLength = 1000.f;

        m_vertices = {
                {origin, xaxis, uvcoord, tangent}, {xaxis*axisLength, xaxis, uvcoord, tangent},
                {origin, yaxis, uvcoord, tangent}, {yaxis*axisLength, yaxis, uvcoord, tangent},
                {origin, zaxis, uvcoord, tangent}, {zaxis*axisLength, zaxis, uvcoord, tangent}
                     };

        m_vbo.submitData(m_vertices.data(), m_vertices.size());

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        m_vbo.bind();
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), BUFFER_OFFSET(sizeof(vec3)));
        glEnableVertexAttribArray(1);


        glBindVertexArray(0);

        VBO::unbind();
    }

    void draw() {
        glBindVertexArray(m_vao);

        glDrawArrays(GL_LINES, 0, 6);

        glBindVertexArray(0);
    }
};

#endif // ORTHOBASE_H
