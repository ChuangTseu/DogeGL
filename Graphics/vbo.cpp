#include "vbo.h"

#include <iostream>

VBO::VBO()
{
    glGenBuffers(1, &m_vboId);
}

VBO::~VBO()
{
    glDeleteBuffers(1, &m_vboId);
}

void VBO::submitData(Vertex *vertex_data, size_t size) {
    bind();

//    std::cerr << "vertex_data address is: " << vertex_data << '\n';

    glBufferData(GL_ARRAY_BUFFER, size*sizeof(Vertex), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size*sizeof(Vertex), vertex_data);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    VBO::unbind();
}

void VBO::submitData(vec3 *vertex_data, size_t size) {
    bind();

//    std::cerr << "vertex_data address is: " << vertex_data << '\n';

    glBufferData(GL_ARRAY_BUFFER, size*sizeof(vec3), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size*sizeof(vec3), vertex_data);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    VBO::unbind();
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
