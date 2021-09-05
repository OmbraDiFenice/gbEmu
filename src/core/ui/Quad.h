#pragma once

#include <core/ui/Buffer.h>
#include <core/ui/opengl/GLVertexBuffer.h>

template <typename Vertex_t>
class Quad {
   public:
    Quad();
    Vertex_t& operator[](const int iIndex);
    void bind() const;
    inline const Buffer& getBuffer() const { return *_vb; }

   private:
    void initBuffers();

   protected:
    static constexpr size_t vertexCount = 4;
    const static std::vector<unsigned int> _indices;
    Vertex_t _vertices[vertexCount];
    std::shared_ptr<Buffer> _vb;
};

template <typename Vertex_t>
const std::vector<unsigned int> Quad<Vertex_t>::_indices{0, 1, 2, 2, 3, 0};

template <typename Vertex_t>
Quad<Vertex_t>::Quad() {
    initBuffers();
}

template <typename Vertex_t>
void Quad<Vertex_t>::bind() const {
    _vb->bind();
}

template <typename Vertex_t>
void Quad<Vertex_t>::initBuffers() {
    const VertexLayout& layout = Vertex_t::ToLayout();

    ASSERT(layout.getStride() == sizeof(Vertex_t),
           "Vertex struct needs to have the same size as a single vertex");

    _vb = std::make_shared<GLVertexBuffer>();
    _vb->setVertexBuffer((float*)(&_vertices[0]),
                         vertexCount * sizeof(Vertex_t));
    _vb->setVertexLayout(layout);
    _vb->setIndexBuffer(_indices);
}

template <typename Vertex_t>
Vertex_t& Quad<Vertex_t>::operator[](const int iIndex) {
    ASSERT(iIndex >= 0, "Quad operator[]: index out of bounds");
    ASSERT(iIndex < vertexCount, "Quad operator[]: index out of bounds");
    return _vertices[iIndex];
}
