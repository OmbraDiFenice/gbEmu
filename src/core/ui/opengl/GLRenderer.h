#pragma once

#include <core/ui/Renderer.h>

class GLRenderer : public Renderer {
   public:
    void draw(const Buffer& iBuffer) const override;
    void clear(float iRed, float iGreen, float iBlue,
               float iAlpha) const override;
};
