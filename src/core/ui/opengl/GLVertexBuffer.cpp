#include <Pch.h>

#include "GLVertexBuffer.h"

#include <utils/GLErrorMacros.h>

GLVertexBuffer GLVertexBuffer::create(
    float* iData, int iVertexNum, const VertexLayout& iLayout,
    const std::vector<unsigned int>& iIndexes) {
    GLVertexBuffer buffer;
    buffer.setVertexBuffer(iData, iVertexNum);
    buffer.setIndexBuffer(iIndexes);
    buffer.setVertexLayout(iLayout);
    return buffer;
}

GLVertexBuffer::GLVertexBuffer() : initialized(false) {
}

void GLVertexBuffer::bind() const {
    if(!initialized) {
        init();
    }

    /* vertex buffer */
    GLCall(glBufferData(GL_ARRAY_BUFFER, _vbCount * sizeof(*_vb), _vb,
                        GL_STATIC_DRAW));

    /* index buffer */
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        _ib.size() * sizeof(_ib.front()), _ib.data(),
                        GL_STATIC_DRAW));
}

void GLVertexBuffer::init() const {
    /* vertex array */
    GLCall(glGenVertexArrays(1, &vertexArray));
    GLCall(glBindVertexArray(vertexArray));

    GLCall(glGenBuffers(1, &vertexBuffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));

    GLCall(glGenBuffers(1, &indexBuffer));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));

    /* vertex array attrib */
    int i      = 0;
    size_t pos = 0;
    for (const VertexElement& element : _layout.getElements()) {
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, GL_FALSE,
                                     _layout.getStride(), (const void*)(pos)));
        ++i;
        pos += element.size();
    }

    initialized = true;
}