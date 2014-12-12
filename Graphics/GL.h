#ifndef GL_H
#define GL_H

#include <cstdint>
#include <GL/glew.h>

#define BUFFER_OFFSET(p) ((uint8_t*)0 + p)

#include <algorithm>

class SafeGLid {
protected:
    GLuint glId = 0;

public:
    SafeGLid() {}

    SafeGLid(const SafeGLid& other) = delete;
    void operator=(const SafeGLid& other) = delete;

    SafeGLid(SafeGLid&& other) { swap(other); }
    void operator=(SafeGLid&& other) { swap(other); }

    GLuint* operator&() { return &glId; }
    GLuint& operator*() { return glId; }
    const GLuint& operator*() const { return glId; }

    GLuint get() const { return glId; }
    GLuint id() const { return glId; }

private:
    void swap(SafeGLid& other) { std::swap(glId, other.glId); }
};


/* MACRO for generating Safe Gluint ids handlers. Might exist a better way through pure template. */
#define GENERATE_GL_TypeId_SafeHandler(ClassName, glGenFunction, glDeleteFunction)      \
    class ClassName : public SafeGLid {                                                 \
    public:                                                                             \
        ClassName() { glGenFunction(1, &glId); }                                        \
        ~ClassName() { glDeleteFunction(1, &glId); }                                    \
        ClassName(const ClassName& other) = delete;                                     \
        void operator=(const ClassName& other) = delete;                                \
        ClassName(ClassName&& other) { std::swap(glId, other.glId); }                   \
        void operator=(ClassName&& other) { std::swap(glId, other.glId); }              \
    };

GENERATE_GL_TypeId_SafeHandler(SafeGlVboId, glGenBuffers, glDeleteBuffers)
GENERATE_GL_TypeId_SafeHandler(SafeGlFboId, glGenFramebuffers, glDeleteFramebuffers)
GENERATE_GL_TypeId_SafeHandler(SafeGlTexId, glGenTextures, glDeleteTextures)

#endif // GL_H
