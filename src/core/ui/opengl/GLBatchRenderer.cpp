#include <Pch.h>

#include <core/ui/opengl/GLBatchRenderer.h>
#include <utils/GLErrorMacros.h>

GLBatchRenderer::GLBatchRenderer(const size_t iBatchSize)
    : _batchSize(iBatchSize), _usedBatch(0), _initialized(false) {}

GLBatchRenderer::~GLBatchRenderer() {
    if (_initialized) {
        delete[] _batchBuffer.getVertexBuffer();
    }
}

void GLBatchRenderer::flush() const {
    ASSERT(_batchBuffer.isLayoutSet(),
           "trying to draw a batch without setting its layout");

    GLRenderer::draw(_batchBuffer);
}

void GLBatchRenderer::draw(const Buffer& iBuffer) const {
    if (!_initialized) {
        initialize(iBuffer);
    }

    if (_usedBatch >= _batchSize) {
        flush();
        _usedBatch = 0;
        _batchBuffer.reset();
    }

    _batchBuffer.addBuffer(iBuffer);
    ++_usedBatch;
}

void GLBatchRenderer::initialize(const Buffer& iBuffer) const {
    const size_t size = _batchSize * iBuffer.getVertexBufferCount();
    auto vertexBuffer = new float[size];
    _batchBuffer.setVertexBuffer(vertexBuffer, size);
    _initialized = true;
}
