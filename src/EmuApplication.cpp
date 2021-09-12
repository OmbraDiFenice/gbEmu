#include <Pch.h>

#include <EmuApplication.h>
#include <core/ui/opengl/GLRenderer.h>

void EmuApplication::init() {
    _renderer = std::make_unique<GLRenderer>();
}

void EmuApplication::drawScreen() {}
