#pragma once

#include <core/ui/Buffer.h>
#include <glm/mat4x4.hpp>

class Renderer {
   public:
    virtual void setBackgroundTileData(void* iData, size_t iSize)    = 0;
    virtual void setBackgroundTileMapData(void* iData, size_t iSize) = 0;
    virtual void drawBackground() const                              = 0;
    virtual void clear(float iRed, float iGreen, float iBlue,
                       float iAlpha) const                           = 0;
    virtual void flush() const                                       = 0;
    virtual const glm::mat4& getProjectionMatrix() const             = 0;
    virtual void setScale(float iScale)                              = 0;
    virtual void setSignedBackgroundTileMap(bool iSigned) const      = 0;

    virtual void drawSprites() const                          = 0;
    virtual void setSpriteTileData(void* iData, size_t iSize) = 0;
    virtual void setOam(void* iData, size_t iSize)            = 0;
};