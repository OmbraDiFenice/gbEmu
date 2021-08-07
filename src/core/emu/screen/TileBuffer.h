#pragma once

#include <core/ui/Buffer.h>
#include <glm/vec3.hpp>
#include <vector>

struct TileVertex {
    glm::vec3 position;
    float tileId;
};

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
    static constexpr size_t vertexCount = 4;
    const static std::vector<unsigned int> _indices;
    const static VertexLayout _layout;
    TileVertex _vertices[vertexCount];
    std::shared_ptr<Buffer> _vb;
};
