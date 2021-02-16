#pragma once

class Tile {
   public:
    explicit Tile();
    explicit Tile(int iX, int iY, int iIndex);
    void setIndex(int iIndex);
    void setPosition(float iX, float iY);

    void bind() const;

   private:
    void setTextureCoords();

   private:
    int _index;
    unsigned int _x;
    unsigned int _y;

    // clang-format off
    float vertices[4 * 5] = {
        0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        1.0f, 1.0f,  0.0f, 1.0f, 1.0f,
        0.0f, 1.0f,  0.0f, 0.0f, 1.0f
    };
    // clang-format on
    const unsigned int indices[6] = {0, 1, 2, 2, 3, 0};
};
