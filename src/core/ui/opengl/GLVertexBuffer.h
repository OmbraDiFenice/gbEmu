#pragma once

#include <core/ui/Buffer.h>

class GLVertexBuffer : public Buffer {
   public:
    virtual ~GLVertexBuffer() = default;

    static GLVertexBuffer create(float* iData, int iVertexNum,
                                 const VertexLayout& iLayout,
                                 const std::vector<unsigned int>& iIndexes);

    void bind() const override;
};
