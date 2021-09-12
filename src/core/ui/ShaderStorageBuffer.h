#pragma once

class ShaderStorageBuffer {
   public:
    virtual ~ShaderStorageBuffer() = default;

    virtual void bind() const                         = 0;
    virtual void unbind() const                       = 0;
    virtual void uploadData(void* iData, size_t iSize,
                            unsigned int iMode) const = 0;
};
