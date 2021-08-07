#pragma once

#include <core/emu/screen/ColorPalette.h>
#include <core/emu/screen/ObjectAttributeMemory.h>
#include <core/emu/screen/TileBuffer.h>
#include <core/emu/screen/VideoItem.h>
#include <core/ui/Program.h>
#include <core/ui/Texture.h>
#include <utility>

class SpriteMap : public VideoItem {
   public:
    SpriteMap();
    void update();
    inline void setOam(const ObjectAttributeMemory& iOam) { _oam = iOam; }
    inline const std::shared_ptr<Texture>& getSpriteTableTexture() const {
        return _spriteTableTexture;
    }
    inline void setSpriteTableTexture(std::shared_ptr<Texture> iTexture) {
        _spriteTableTexture = std::move(iTexture);
    };
    inline const std::unique_ptr<Program>& getProgram() const {
        return _program;
    }

    void render(const GbRenderer& renderer) const override;

   private:
    void initializeProgram();

   private:
    TileBuffer _spritePatternTable[16 * 16];
    ColorPalette _colorMap;
    TileBuffer _spritesToBeDrawn[40];  // we need to keep a buffer different
                                       // from the spritePatternTable because
                                       // the same pattern could be drawn
                                       // multiple times at different locations
    ObjectAttributeMemory _oam;
    std::shared_ptr<Texture> _spriteTableTexture;
    std::unique_ptr<Program> _program;
};