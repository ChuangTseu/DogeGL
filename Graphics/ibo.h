#ifndef IBO_H
#define IBO_H

#include "GL.h"

class IBO
{
    GLuint m_iboId;
public:
    IBO();

    ~IBO();

    void submitData(GLuint* indices_data, size_t size);

    void bind();

    static void unbind();
};

#endif // IBO_H
