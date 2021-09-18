#include <Pch.h>
#include <core/emu/utils.h>
#include <core/ui/opengl/GLRenderer.h>

#include "SandboxApplication.h"

SandboxApplication::SandboxApplication() {}

void SandboxApplication::init() {
    _renderer = std::make_unique<GLRenderer>();
    _renderer->setScale(1.f);

    // load background data
    unsigned char* tileMapPatterns = loadData("tileDataTable_8800.DMP");
    _renderer->setBackgroundTileData(tileMapPatterns, 4096);
    delete[] tileMapPatterns;

    unsigned char* tileMap = loadData("tileMap_9800.DMP");
    _renderer->setBackgroundTileMapData(tileMap, 1024);
    _renderer->setSignedBackgroundTileMap(true);
    delete[] tileMap;

    // load sprite data
    unsigned char* spritePatterns = loadData("spriteDataTable_8000.DMP");
    _renderer->setSpriteTileData(spritePatterns, 4096);
    delete[] spritePatterns;
}

void SandboxApplication::drawScreen() {
    _renderer->drawBackground();
    _renderer->drawSprites();
}
