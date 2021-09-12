#include <Pch.h>
#include <core/emu/utils.h>
#include <core/ui/opengl/GLRenderer.h>

#include "SandboxApplication.h"

SandboxApplication::SandboxApplication() {}

void SandboxApplication::init() {
    _renderer = std::make_unique<GLRenderer>();
    _renderer->setScale(.5f);

    unsigned char* tileMapPatterns = loadData("tileDataTable_8800.DMP");
    _renderer->setBackgroundTileData(tileMapPatterns, 4096);
    delete[] tileMapPatterns;
}

void SandboxApplication::drawScreen() { _renderer->drawBackground(); }
