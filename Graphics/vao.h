#ifndef VAO_H
#define VAO_H

#include "GL.h"

class VAO
{
    GLuint m_vaoId;
public:
    VAO();

    ~VAO();

    void bind();

    static void unbind();
};

#endif // VAO_H
