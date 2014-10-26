#ifndef IBO_H
#define IBO_H

#include <GL/glew.h>

class IBO
{
    GLuint m_iboId;
public:
    IBO();

    void submitData(GLuint* indices_data, size_t size);

    void bind();

    static void unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};

#endif // IBO_H
