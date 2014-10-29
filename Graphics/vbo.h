#ifndef VBO_H
#define VBO_H

#include "GL.h"

#include "vertex.h"

class VBO
{
    GLuint m_vboId;
public:
    VBO();

    void submitData(Vertex* vertex_data, size_t size);

    void bind();

    static void unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

#endif // VBO_H
