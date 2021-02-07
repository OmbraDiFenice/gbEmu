#pragma once
#include <glad/gl.h>
#include <utils/Assert.h>
#include <utils/Logging.h>

#define GLCall(glFunc) \
    GLClearErrors();   \
    glFunc;            \
    GLLogError(#glFunc, __FILE__, __LINE__)

static void GLClearErrors() {
    while (glGetError() != GL_NO_ERROR)
        ;
}

static void GLLogError(const char* iFunction, const char* iFile, int iLine) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG_ERROR("[OpenGL error] (" << error << ") on " << iFunction << " at "
                                     << iFile << ":" << iLine);
    }
}
