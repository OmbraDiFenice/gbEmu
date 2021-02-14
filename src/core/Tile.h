#pragma once

class Tile {
   public:
    explicit Tile(int iIndex = 0);
    void setIndex(int iIndex);

    void bind() const;

   private:
    void setTextureCoords();

   private:
    int _index;

    // clang-format off
    float vertices[4 * 5] = {
        -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f, 0.0f, 1.0f
    };
    // clang-format on
    const unsigned int indices[6] = {0, 1, 2, 2, 3, 0};
};
