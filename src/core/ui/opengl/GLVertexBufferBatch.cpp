#include <Pch.h>

#include "GLVertexBufferBatch.h"

GLVertexBufferBatch::GLVertexBufferBatch() { reset(); }

bool GLVertexBufferBatch::isLayoutCompatible(
    const VertexLayout& iLayout) const {
    return !_isLayoutInitialized || _layout == iLayout;
}

void GLVertexBufferBatch::addBuffer(const Buffer& iBuffer) {
    setVertexLayout(iBuffer.getLayout());

    memcpy(_nextVertexBuffer, iBuffer.getVertexBuffer(),
           iBuffer.getVertexBufferSize());
    _nextVertexBuffer += iBuffer.getVertexBufferCount();

    for (auto index : iBuffer.getIndexBuffer()) {
        _ib.push_back(index + _indexOffset);
    }

    // the following assumes that each vertex in vertex buffer is referenced by
    // at least 1 index in the index buffer
    auto [min, max] = std::minmax_element(iBuffer.getIndexBuffer().begin(),
                                          iBuffer.getIndexBuffer().end());
    _indexOffset += (max == iBuffer.getIndexBuffer().end() ? 0 : *max) -
                    (min == iBuffer.getIndexBuffer().end() ? 0 : *min) + 1;
}

void GLVertexBufferBatch::setVertexLayout(const VertexLayout& iLayout) {
    if (!_isLayoutInitialized) {
        GLVertexBuffer::setVertexLayout(iLayout);
        _isLayoutInitialized = true;
    } else {
        ASSERT(isLayoutCompatible(iLayout),
               "trying to batch render buffers with incompatible layout");
    }
}

void GLVertexBufferBatch::setVertexBuffer(float* iVertexBuffer,
                                          size_t iBufferCount) {
    Buffer::setVertexBuffer(iVertexBuffer, iBufferCount);
    _nextVertexBuffer = _vb;
}

void GLVertexBufferBatch::reset() {
    _isLayoutInitialized = false;
    _indexOffset         = 0;
    _nextVertexBuffer    = _vb;
}