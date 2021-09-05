#pragma once

#include <core/ui/ShaderStorageBuffer.h>
#include "glad/gl.h"

class GLShaderStorageBuffer : public ShaderStorageBuffer {
   public:
    GLShaderStorageBuffer();
    virtual ~GLShaderStorageBuffer() = default;

    void bind() const override;
    void unbind() const override;
    void uploadData(void* iData, size_t iSize, GLenum iMode) const override;

   private:
    void init();

   private:
    GLuint _ref = 0;
};
