#pragma once

class ColorPalette {
   public:
    explicit ColorPalette(
        std::initializer_list<std::initializer_list<unsigned int>> data);

    void setRed(unsigned int iIndex, unsigned int iValue);
    void setGreen(unsigned int iIndex, unsigned int iValue);
    void setBlue(unsigned int iIndex, unsigned int iValue);
    void setAlpha(unsigned int iIndex, unsigned int iValue);

    operator float*() { return &_data[0][0]; }

   private:
    inline float normalize(unsigned int iValue) { return iValue / 255.0f; }

   private:
    float _data[4][4];
};
