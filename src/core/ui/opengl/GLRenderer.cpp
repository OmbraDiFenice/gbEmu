#include <Pch.h>

#include "GLRenderer.h"

#include <utils/GLErrorMacros.h>

void GLRenderer::draw(const Buffer& iBuffer) const {
    if (_batchActive) {
        drawBatched(iBuffer);
    } else {
        drawImmediate(iBuffer);
    }
}

void GLRenderer::clear(float iRed, float iGreen, float iBlue,
                       float iAlpha) const {
    GLCall(glClearColor(iRed, iGreen, iBlue, iAlpha));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void GLRenderer::startBatch(size_t iCount) const {
    auto vertexBuffer = new float[iCount];
    _batchBuffer.setVertexBuffer(vertexBuffer, iCount);
    _batchActive = true;
}

void GLRenderer::endBatch() const {
    ASSERT(_batchBuffer.isLayoutSet(),
           "trying to draw a batch without setting its layout");

    _batchBuffer.bind();
    GLCall(glDrawElements(GL_TRIANGLES, _batchBuffer.getIndexBuffer().size(),
                          GL_UNSIGNED_INT, nullptr));
    delete[] _batchBuffer.getVertexBuffer();
    _batchActive = false;
}

void GLRenderer::drawImmediate(const Buffer& iBuffer) const {
    iBuffer.bind();
    GLCall(glDrawElements(GL_TRIANGLES, iBuffer.getIndexBuffer().size(),
                          GL_UNSIGNED_INT, nullptr));
}

void GLRenderer::drawBatched(const Buffer& iBuffer) const {
    _batchBuffer.addBuffer(iBuffer);
}
