#pragma once

#include <core/ui/opengl/GLVertexBuffer.h>
#include <vector>
#include <core/ui/Buffer.h>

class Tile {
   public:
    explicit Tile();
    explicit Tile(int iX, int iY, int iIndex);
    void setIndex(int iIndex);
    void setPosition(float iX, float iY);
    inline const Buffer& getVertexBuffer() { return _vb; }

    void bind() const;

   private:
    void initBuffers(int iX, int iY, int iIndex);
    void setTextureCoords(int iIndex);

   private:
    // clang-format off
    float _vertices[4 * 5] = {
        0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        1.0f, 1.0f,  0.0f, 1.0f, 1.0f,
        0.0f, 1.0f,  0.0f, 0.0f, 1.0f
    };
    // clang-format on

    const static std::vector<unsigned int> _indices;
    const static VertexLayout _layout;
    GLVertexBuffer _vb;
};
