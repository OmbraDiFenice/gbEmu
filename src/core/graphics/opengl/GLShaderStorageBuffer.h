#pragma once

#include <core/graphics/ShaderStorageBuffer.h>
#include <glad/gl.h>

class GLShaderStorageBuffer : public ShaderStorageBuffer {
   public:
    GLShaderStorageBuffer(GLuint iBindingIndex);
    virtual ~GLShaderStorageBuffer() = default;

    void bind() const override;
    void unbind() const override;
    void uploadData(void* iData, size_t iSize, GLenum iMode) const override;

   private:
    GLuint _ref          = 0;
    GLuint _bindingIndex = 0;
};
