#include <Pch.h>
#include <core/emu/utils.h>
#include <core/ui/opengl/GLRenderer.h>

#include "SandboxApplication.h"

SandboxApplication::SandboxApplication() {}

SandboxApplication::~SandboxApplication() {
    delete[] tileMapPatterns;
    delete[] tileMap;
    delete[] spritePatterns;
    delete[] oam;
}

void SandboxApplication::init() {
    _renderer = std::make_unique<GLRenderer>();
    _renderer->setScale(1.f);

    tileMapPatterns = loadData("tileDataTable_8800.DMP");
    tileMap         = loadData("tileMap_9800.DMP");
    spritePatterns  = loadData("spriteDataTable_8000.DMP");
    oam             = loadData("OAM_FE00.DMP");
}

void SandboxApplication::update() {
    _renderer->setBackgroundTileData(tileMapPatterns, 4096);
    _renderer->setBackgroundTileMapData(tileMap, 1024);
    _renderer->setSignedBackgroundTileMap(true);
    _renderer->setSpriteTileData(spritePatterns, 4096);
    _renderer->setOam(oam, 40 * 4);
}

void SandboxApplication::drawScreen() {
    _renderer->drawBackground();
    _renderer->drawSprites();
}
