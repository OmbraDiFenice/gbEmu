#pragma once

#include <core/ui/Buffer.h>

class GLVertexBuffer : public Buffer {
   public:
    GLVertexBuffer();
    virtual ~GLVertexBuffer() = default;

    static GLVertexBuffer create(float* iData, int iVertexNum,
                                 const VertexLayout& iLayout,
                                 const std::vector<unsigned int>& iIndexes);

    void bind() const override;

   private:
    void init() const;

   private:
    mutable GLuint vertexArray;
    mutable GLuint vertexBuffer;
    mutable GLuint indexBuffer;
    mutable bool initialized;
};
