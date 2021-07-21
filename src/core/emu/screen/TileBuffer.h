#pragma once

#include <vector>
#include <core/ui/Buffer.h>

class TileBuffer {
   public:
    explicit TileBuffer();
    explicit TileBuffer(int iX, int iY, int iIndex);
    void setPosition(float iX, float iY);
    void setTileIndex(int iIndex);
    inline const Buffer& getVertexBuffer() const { return *_vb; }

    void bind() const;

   private:
    void initBuffers(int iX, int iY, int iIndex);

   private:
    // clang-format off
    float _vertices[4 * 4] = {  // TODO: the size should somehow depend on the layout, at least having a check on the size to signal potential errors when we add new data
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f
    };
    // clang-format on

    const static std::vector<unsigned int> _indices;
    const static VertexLayout _layout;
    std::shared_ptr<Buffer> _vb;
};
