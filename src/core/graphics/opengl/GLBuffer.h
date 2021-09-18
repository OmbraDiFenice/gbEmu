#pragma once

#include <core/graphics/Buffer.h>

/** Represents an OpenGl vertex buffer loaded on the GPU
 *
 */
class GLVertexBuffer : public VertexBuffer {
   public:
    GLVertexBuffer(size_t iSize);
    GLVertexBuffer(void* iData, size_t iSize);
    virtual ~GLVertexBuffer();

    void setData(void* iData, size_t iSize) const override;
    void bind() const override;
    void unbind() const override;

   private:
    void init(void* iData, size_t iSize);

   private:
    GLuint _id;
};

/** Represents an OpenGl index buffer loaded on the GPU
 *
 */
class IndexBuffer {
   public:
    IndexBuffer(uint32_t* iData, size_t iCount);
    virtual ~IndexBuffer();

    virtual void bind();
    virtual void unbind();

    inline uint32_t getCount() const { return _count; }

   private:
    GLuint _id;
    uint32_t _count;
};

/** Represents an OpenGl vertex array buffer loaded on the GPU
 *
 */
class GLVertexArray {
   public:
    GLVertexArray();
    virtual ~GLVertexArray();

    void bind() const;
    void unbind() const;

    void setIndexBuffer(const std::shared_ptr<IndexBuffer>& iIndexBuffer);
    void addVertexBuffer(const std::shared_ptr<VertexBuffer>& iVertexBuffer);
    void clearVertexBuffer();

    const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const {
        return _vb;
    }
    const std::shared_ptr<IndexBuffer>& getIndexBuffer() const { return _ib; }

   private:
    GLuint _id;
    std::vector<std::shared_ptr<VertexBuffer>> _vb;
    std::shared_ptr<IndexBuffer> _ib;
    GLuint _lastVertexBufferIndex;
};
