#include <Pch.h>

#include "GLRenderer.h"

#include <utils/GLErrorMacros.h>

void GLRenderer::draw(const Buffer& iBuffer) const {
    iBuffer.bind();
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

void GLRenderer::clear(float iRed, float iGreen, float iBlue,
                       float iAlpha) const {
    GLCall(glClearColor(iRed, iGreen, iBlue, iAlpha));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
