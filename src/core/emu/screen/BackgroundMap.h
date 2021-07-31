#pragma once

#include <core/emu/screen/TileBuffer.h>
#include <core/emu/screen/VideoItem.h>
#include <core/ui/Program.h>
#include <core/ui/Texture.h>
#include <utility>

class BackgroundMap : VideoItem {
   public:
    BackgroundMap();
    void reindex(const unsigned char* iBackgroundTileMap, bool iSignedIndexes);
    inline const std::shared_ptr<Texture>& getBackgroundTableTexture() const {
        return _backgroundTableTexture;
    }
    inline void setBackgroundTableTexture(std::shared_ptr<Texture> iTexture) {
        _backgroundTableTexture = std::move(iTexture);
    }
    inline const std::unique_ptr<Program>& getProgram() const {
        return _program;
    }

    void render(const GbRenderer& renderer) const override;
    void initialize() override;

   private:
    TileBuffer backgroundMap[32][32];
    std::shared_ptr<Texture> _backgroundTableTexture;
    std::unique_ptr<Program> _program;
};
