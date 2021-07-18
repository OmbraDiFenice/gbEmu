#pragma once

#include <utility>

#include "glad/gl.h"

class Texture {
   public:
    explicit Texture(const std::string& iPath) : _data(nullptr) {}
    Texture(std::shared_ptr<unsigned char[]> iData, int iWidth, int iHeight)
        : _data(std::move(iData)), _width(iWidth), _height(iHeight) {}
    virtual ~Texture() = default;

    virtual void bind(unsigned int slot) const = 0;
    virtual void unbind() const = 0;

   protected:
    std::shared_ptr<unsigned char[]> _data;
    int _width;
    int _height;
};
