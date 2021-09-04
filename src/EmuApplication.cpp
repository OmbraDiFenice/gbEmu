#include <Pch.h>

#include <EmuApplication.h>
#include <core/emu/screen/TilePatternAdapter.h>

void EmuApplication::init() {
    TilePatternAdapter tileMapPatternAdapter;
    _video = std::make_unique<Video>(tileMapPatternAdapter);
    _video->update();

    _renderer = std::make_unique<GbRenderer>();
}

void EmuApplication::drawScreen() { _renderer->draw(*_video); }
