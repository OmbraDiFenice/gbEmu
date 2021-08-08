#include "Pch.h"

#include <core/emu/screen/GbRenderer.h>
#include <core/ui/opengl/GLBatchRenderer.h>
#include <core/emu/screen/BackgroundMap.h>
#include <core/emu/screen/SpriteMap.h>
#include <core/emu/screen/Video.h>

GbRenderer::GbRenderer() { _renderer = std::make_unique<GLRenderer>(); }

void GbRenderer::draw(const Video& iVideo) const { iVideo.render(*this); }

void GbRenderer::draw(const BackgroundMap& iBackgroundMap) const {
    iBackgroundMap.render(*this);
}

void GbRenderer::draw(const SpriteMap& iSpriteMap) const {
    iSpriteMap.render(*this);
}
