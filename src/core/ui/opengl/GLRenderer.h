#pragma once

#include <core/ui/Renderer.h>
#include <core/ui/opengl/GLVertexBufferBatch.h>

class GLRenderer : public Renderer {
   public:
    void draw(const Buffer& iBuffer) const override;
    void clear(float iRed, float iGreen, float iBlue,
               float iAlpha) const override;
    void startBatch(size_t iCount) const override;
    void endBatch() const override;

   private:
    void drawImmediate(const Buffer& iBuffer) const;
    void drawBatched(const Buffer& iBuffer) const;

   private:
    mutable GLVertexBufferBatch _batchBuffer;
    mutable bool _batchActive = false;
};
