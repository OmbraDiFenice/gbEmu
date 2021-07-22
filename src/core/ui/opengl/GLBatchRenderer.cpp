#include <Pch.h>

#include <core/ui/opengl/GLBatchRenderer.h>

GLBatchRenderer::GLBatchRenderer(const size_t iBatchSize)
    : _batchSize(iBatchSize), _usedBatch(0), _initialized(false) {
    ASSERT(_batchSize > 0, "invalid batch size");
}

GLBatchRenderer::~GLBatchRenderer() {
    if (_initialized) {
        delete[] _batchBuffer.getVertexBuffer();
    }
}

void GLBatchRenderer::flush() const {
    if(_usedBatch == 0) return;

    ASSERT(_batchBuffer.isLayoutSet(),
           "trying to draw a batch without setting its layout");

    _batchBuffer.bind();
    GLRenderer::draw(_batchBuffer);
    _usedBatch = 0;
    _batchBuffer.reset();
}

void GLBatchRenderer::draw(const Buffer& iBuffer) const {
    if (!_initialized) {
        initialize(iBuffer);
    }

    _batchBuffer.addBuffer(iBuffer);
    ++_usedBatch;

    if (_usedBatch >= _batchSize) {
        flush();
    }
}

void GLBatchRenderer::initialize(const Buffer& iBuffer) const {
    const size_t maxSize = _batchSize * iBuffer.getVertexBufferCount();
    auto vertexBuffer = new float[maxSize];
    _batchBuffer.setVertexBuffer(vertexBuffer, maxSize);
    _initialized = true;
}
