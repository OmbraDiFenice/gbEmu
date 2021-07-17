#pragma once

#include <core/ui/opengl/GLRenderer.h>
#include <core/ui/opengl/GLVertexBufferBatch.h>

class GLBatchRenderer : public GLRenderer {
   public:
    explicit GLBatchRenderer(size_t iBatchSize);
    ~GLBatchRenderer();

    void draw(const Buffer& iBuffer) const override;
    void flush() const override;

   private:
    void initialize(const Buffer& iBuffer) const;

   private:
    mutable GLVertexBufferBatch _batchBuffer;
    const size_t _batchSize;
    mutable size_t _usedBatch;
    mutable bool _initialized;
    void initialize(const Buffer& iBuffer);
};