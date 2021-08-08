#pragma once

#include <core/ui/Renderer.h>

class Video;
class BackgroundMap;
class SpriteMap;

class GbRenderer : public Renderer {
   public:
    GbRenderer();
    void draw(const Video& iVideo) const;
    void draw(const BackgroundMap& iBackgroundMap) const;
    void draw(const SpriteMap& iSpriteMap) const;

    void draw(const Buffer& iBuffer) const override {
        _renderer->draw(iBuffer);
    }
    void clear(float iRed, float iGreen, float iBlue,
               float iAlpha) const override {
        _renderer->clear(iRed, iGreen, iBlue, iAlpha);
    }
    void flush() const override { _renderer->flush(); }

   private:
    std::unique_ptr<Renderer> _renderer;
};