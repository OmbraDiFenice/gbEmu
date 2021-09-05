#pragma once

#include <glm/mat4x4.hpp>

class ColorPalette {
   public:
    explicit ColorPalette(const glm::mat4& data);
    explicit ColorPalette(const glm::mat4&& data);

    void setRed(unsigned int iIndex, unsigned int iValue);
    void setGreen(unsigned int iIndex, unsigned int iValue);
    void setBlue(unsigned int iIndex, unsigned int iValue);
    void setAlpha(unsigned int iIndex, unsigned int iValue);

    operator float*() { return &_data[0][0]; }

   private:
    inline float normalize(unsigned int iValue);
    inline void normalize();

   private:
    glm::mat4 _data;
};
