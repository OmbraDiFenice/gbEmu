#pragma once

#include <core/ui/Quad.h>
#include <glm/vec3.hpp>

struct TileVertex {
    glm::vec3 position;
    float tileId;

    static const VertexLayout& ToLayout() {
        // clang-format off
        static VertexLayout layout{
            {3, GL_FLOAT},
            {1, GL_FLOAT}
        };
        // clang-format on
        return layout;
    }
};

class TileBuffer : public Quad<TileVertex> {
   public:
    explicit TileBuffer();
    explicit TileBuffer(int iX, int iY, int iIndex);
    void setPosition(float iX, float iY);
    void setTileIndex(int iIndex);

   private:
    void init(int iX, int iY, int iIndex);
};
