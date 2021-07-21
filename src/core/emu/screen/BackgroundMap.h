#pragma once

#include <core/emu/screen/VideoItem.h>
#include <core/emu/screen/TileBuffer.h>
#include <core/ui/Texture.h>

class BackgroundMap : VideoItem {
   public:
    BackgroundMap();
    void reindex(const unsigned char* iBackgroundTileMap, bool iSignedIndexes);
    inline const std::shared_ptr<Texture>& getBackgroundTableTexture() const {
        return _backgroundTableTexture;
    }
    inline void setBackgroundTableTexture(std::shared_ptr<Texture> iTexture) { _backgroundTableTexture = iTexture; };

    void render(const GbRenderer& renderer) const override;

   private:
    TileBuffer backgroundMap[32][32];
    std::shared_ptr<Texture> _backgroundTableTexture;
};
