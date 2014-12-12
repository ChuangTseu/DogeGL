#include "ibo.h"

IBO::IBO()
{
    glGenBuffers(1, &m_iboId);
}

IBO::~IBO()
{
    glDeleteBuffers(1, &m_iboId);
}

void IBO::submitData(GLuint* indices_data, size_t size) {
    bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size*sizeof(GLuint), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size*sizeof(GLuint), indices_data);

    IBO::unbind();
}

void IBO::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
}

void IBO::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
