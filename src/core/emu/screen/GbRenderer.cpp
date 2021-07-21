#include "Pch.h"

#include <core/emu/screen/GbRenderer.h>
#include <core/ui/opengl/GLBatchRenderer.h>
#include <core/emu/screen/BackgroundMap.h>
#include <core/emu/screen/SpriteMap.h>
#include <core/emu/screen/Video.h>

GbRenderer::GbRenderer() {
    _renderer = std::make_unique<GLBatchRenderer>(32*32);
}

void GbRenderer::render(const Video& iVideo) const {
    iVideo.render(*this);
}

void GbRenderer::render(const BackgroundMap& iBackgroundMap) const {
    iBackgroundMap.render(*this);
}

void GbRenderer::render(const SpriteMap& iSpriteMap) const {
    iSpriteMap.render(*this);
}
