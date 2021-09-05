#include <Pch.h>

#include <utils/GLErrorMacros.h>
#include "GLShaderStorageBuffer.h"

GLShaderStorageBuffer::GLShaderStorageBuffer() { init(); }

void GLShaderStorageBuffer::bind() const {
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ref));
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _ref));
}

void GLShaderStorageBuffer::unbind() const {
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

void GLShaderStorageBuffer::uploadData(void *iData, size_t iSize,
                                       GLenum iMode) const {
    bind();
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, iSize, iData, iMode));
    unbind();
}

void GLShaderStorageBuffer::init() { GLCall(glGenBuffers(1, &_ref)); }
