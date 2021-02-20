#pragma once

#include <core/ui/Buffer.h>

class GLVertexBuffer : public Buffer {
   public:
    void bind() const override;
};
