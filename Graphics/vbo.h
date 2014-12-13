#ifndef VBO_H
#define VBO_H

#include "GL.h"

#include "vertex.h"

class VBO
{
    GLuint m_vboId;
public:
    VBO();

    ~VBO();

    void submitData(Vertex* vertex_data, size_t size);
    void submitData(vec3 *vertex_data, size_t size);

    void bind();

    static void unbind();

};

#endif // VBO_H
