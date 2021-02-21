#pragma once

#include <core/ui/Buffer.h>
#include <core/ui/opengl/GLVertexBuffer.h>

class GLVertexBufferBatch : public GLVertexBuffer {
   public:
    GLVertexBufferBatch();

    bool isLayoutCompatible(const VertexLayout& iLayout) const;
    void setVertexLayout(const VertexLayout& iLayout) override;
    void setVertexBuffer(float* iVertexBuffer, size_t iBufferCount) override;
    void addBuffer(const Buffer& iBuffer);

    inline bool isLayoutSet() const { return _isLayoutInitialized; }

   protected:
    bool _isLayoutInitialized = false;
    float* _nextVertexBuffer;
    unsigned int _indexOffset;
};
