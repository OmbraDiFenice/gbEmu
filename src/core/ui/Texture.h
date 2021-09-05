#pragma once

#include <utility>

#include "glad/gl.h"

class Texture {
   public:
    explicit Texture(const std::string& iPath, unsigned int iTextureSlot)
        : _data(nullptr), _textureSlot(iTextureSlot) {
        ASSERT(_textureSlot < 32, "slot has to be less than 32");
    }
    Texture(std::shared_ptr<unsigned char[]> iData, int iWidth, int iHeight,
            unsigned int iTextureSlot)
        : _data(std::move(iData)),
          _width(iWidth),
          _height(iHeight),
          _textureSlot(iTextureSlot) {
        ASSERT(_textureSlot < 32, "slot has to be less than 32");
    }
    virtual ~Texture() = default;

    virtual void bind() const   = 0;
    virtual void unbind() const                                             = 0;
    virtual void associateToWritableBuffer(GLuint iWritableBufferRef) const = 0;
    inline unsigned int getTextureSlot() const { return _textureSlot; };

   protected:
    std::shared_ptr<unsigned char[]> _data;
    int _width;
    int _height;
    unsigned int _textureSlot;
};
