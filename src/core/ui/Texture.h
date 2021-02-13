#pragma once

#include "glad/gl.h"

class Texture {
   public:
    explicit Texture(const std::string& iPath) : _data(nullptr) {}
    Texture(unsigned char* iData, int iWidth, int iHeight)
        : _data(iData), _width(iWidth), _height(iHeight) {}
    virtual ~Texture() = default;

    virtual void bind(unsigned int slot) const = 0;
    virtual void unbind() const = 0;

   protected:
    unsigned char* _data;
    int _width;
    int _height;
};
