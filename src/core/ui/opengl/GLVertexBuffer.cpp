#include <Pch.h>

#include "GLVertexBuffer.h"

#include <utils/GLErrorMacros.h>

void GLVertexBuffer::bind() const {
    /* vertex array */
    GLuint vertexArray;
    GLCall(glGenVertexArrays(1, &vertexArray));
    GLCall(glBindVertexArray(vertexArray));

    /* vertex buffer */
    GLuint vertexBuffer;
    GLCall(glGenBuffers(1, &vertexBuffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, _vbCount * sizeof(*_vb), _vb,
                        GL_STATIC_DRAW));

    /* index buffer */
    GLuint indexBuffer;
    GLCall(glGenBuffers(1, &indexBuffer));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        _ib.size() * sizeof(_ib.front()), _ib.data(),
                        GL_STATIC_DRAW));

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
}
