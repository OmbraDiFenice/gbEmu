#pragma once

#include <core/emu/screen/TileBuffer.h>
#include <core/emu/screen/VideoItem.h>
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

    void render(const GbRenderer& renderer) const override;

   private:
    TileBuffer spriteMap[16][16];
    std::shared_ptr<Texture> _spriteTableTexture;
};