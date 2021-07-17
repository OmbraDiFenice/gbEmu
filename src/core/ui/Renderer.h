#pragma once

#include <core/ui/Buffer.h>

class Renderer {
   public:
    virtual void draw(const Buffer& iBuffer) const = 0;
    virtual void clear(float iRed, float iGreen, float iBlue,
                       float iAlpha) const         = 0;
    virtual void flush() const = 0;
};