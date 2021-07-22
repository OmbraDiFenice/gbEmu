#pragma once

#include <core/emu/screen/TileBuffer.h>
#include <core/emu/screen/VideoItem.h>
#include <core/ui/Program.h>
#include <core/ui/Texture.h>
#include <utility>

class SpriteMap : public VideoItem {
   public:
    SpriteMap();
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
    TileBuffer spriteMap[16][16];
    std::shared_ptr<Texture> _spriteTableTexture;
    std::unique_ptr<Program> _program;
};