#include <Pch.h>

#include <EmuApplication.h>
#include <core/emu/utils.h>
#include <core/ui/opengl/GLRenderer.h>

EmuApplication::~EmuApplication() {
    delete[] tileMapPatterns;
    delete[] tileMap;
    delete[] spritePatterns;
    delete[] oam;
}

void EmuApplication::init() {
    _renderer = std::make_unique<GLRenderer>();

    tileMapPatterns = loadData("tileDataTable_8800.DMP");
    tileMap         = loadData("tileMap_9800.DMP");
    spritePatterns  = loadData("spriteDataTable_8000.DMP");
    oam             = loadData("OAM_FE00.DMP");
}

void EmuApplication::update() {
    _renderer->setBackgroundTileData(tileMapPatterns, 4096);
    _renderer->setBackgroundTileMapData(tileMap, 1024);
    _renderer->setSignedBackgroundTileMap(true);
    _renderer->setSpriteTileData(spritePatterns, 4096);
    _renderer->setOam(oam, 40 * 4);
}

void EmuApplication::drawScreen() {
    _renderer->clear(0.9f, 0.9f, 0.9f, 1.0f);
    _renderer->drawBackground();
    _renderer->drawSprites();
}
