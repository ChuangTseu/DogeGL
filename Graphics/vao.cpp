#include "vao.h"

VAO::VAO()
{
    glGenVertexArrays(1, &m_vaoId);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &m_vaoId);
}

void VAO::bind() {
    glBindVertexArray(m_vaoId);
}

void VAO::unbind() {
    glBindVertexArray(0);
}
