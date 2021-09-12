#include <Pch.h>

#include <core/ui/opengl/GLBuffer.h>
#include <utils/GLErrorMacros.h>

// ----------------- Vertex buffer -----------------

GLVertexBuffer::GLVertexBuffer(size_t iSize) { init(nullptr, iSize); }

GLVertexBuffer::GLVertexBuffer(void* iData, size_t iSize) {
    init(iData, iSize);
}

GLVertexBuffer::~GLVertexBuffer() { GLCall(glDeleteBuffers(1, &_id)); }

void GLVertexBuffer::setData(void* iData, size_t iSize) const {
    bind();
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, iSize, iData));
}

void GLVertexBuffer::bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, _id));
}

void GLVertexBuffer::unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void GLVertexBuffer::init(void* iData, size_t iSize) {
    GLCall(glGenBuffers(1, &_id));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, _id));
    GLCall(glBufferData(GL_ARRAY_BUFFER, iSize, iData, GL_STATIC_DRAW));
}

// ----------------- Index buffer -----------------

IndexBuffer::IndexBuffer(uint32_t* iData, size_t iCount) : _count(iCount) {
    GLCall(glGenBuffers(1, &_id));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, _id));
    GLCall(glBufferData(GL_ARRAY_BUFFER, iCount * sizeof(uint32_t), iData,
                        GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() { GLCall(glDeleteBuffers(1, &_id)); }

void IndexBuffer::bind() { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id)); }

void IndexBuffer::unbind() { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

// ----------------- Vertex Array -----------------

GLVertexArray::GLVertexArray() : _lastVertexBufferIndex(0), _ib(nullptr) {
    GLCall(glGenVertexArrays(1, &_id));
}

GLVertexArray::~GLVertexArray() { GLCall(glDeleteVertexArrays(1, &_id)); }

void GLVertexArray::bind() const { GLCall(glBindVertexArray(_id)); }

void GLVertexArray::unbind() const { GLCall(glBindVertexArray(0)); }

void GLVertexArray::setIndexBuffer(
    const std::shared_ptr<IndexBuffer>& iIndexBuffer) {
    bind();
    iIndexBuffer->bind();
    _ib = iIndexBuffer;
}

void GLVertexArray::addVertexBuffer(
    const std::shared_ptr<VertexBuffer>& iVertexBuffer) {
    bind();
    iVertexBuffer->bind();

    const VertexLayout& vbLayout = iVertexBuffer->getLayout();

    size_t pos = 0;
    for (const VertexElement& element : vbLayout.getElements()) {
        GLCall(glEnableVertexAttribArray(_lastVertexBufferIndex));
        switch (element.type) {
            case GL_FLOAT:
                GLCall(glVertexAttribPointer(
                    _lastVertexBufferIndex, element.count, element.type,
                    GL_FALSE, vbLayout.getStride(), (const void*)(pos)));
                break;
            case GL_UNSIGNED_INT:
            case GL_INT:
                GLCall(glVertexAttribIPointer(
                    _lastVertexBufferIndex, element.count, element.type,
                    vbLayout.getStride(), (const void*)(pos)));
                break;
        }
        ++_lastVertexBufferIndex;
        pos += element.size();
    }

    _vb.emplace_back(iVertexBuffer);
}

void GLVertexArray::clearVertexBuffer() { _vb.clear(); }
