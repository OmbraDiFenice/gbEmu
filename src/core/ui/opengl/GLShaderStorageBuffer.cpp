#include <Pch.h>

#include <core/ui/opengl/GLShaderStorageBuffer.h>
#include <utils/GLErrorMacros.h>

GLShaderStorageBuffer::GLShaderStorageBuffer(GLuint iBindingIndex) {
    GLCall(glGenBuffers(1, &_ref));
    _bindingIndex = iBindingIndex;
}

void GLShaderStorageBuffer::bind() const {
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _bindingIndex, _ref));
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
