#include <Pch.h>

#include <core/emu/screen/ColorPalette.h>

ColorPalette::ColorPalette(
    std::initializer_list<std::initializer_list<unsigned int>> data) {
    ASSERT(data.size() == 4, "color palette requires 4 colors");
    auto colorIt = data.begin();
    for (int color = 0; color < 4; ++color, ++colorIt) {
        ASSERT(colorIt->size() == 4,
               "color palette requires 4 component per color");
        auto componentIt = colorIt->begin();
        for (int component = 0; component < 4; ++component, ++componentIt) {
            _data[color][component] = normalize(*componentIt);
        }
    }
}

void ColorPalette::setRed(unsigned int iIndex, unsigned int iValue) {
    _data[iIndex][0] = normalize(iValue);
}

void ColorPalette::setGreen(unsigned int iIndex, unsigned int iValue) {
    _data[iIndex][1] = normalize(iValue);
}

void ColorPalette::setBlue(unsigned int iIndex, unsigned int iValue) {
    _data[iIndex][2] = normalize(iValue);
}

void ColorPalette::setAlpha(unsigned int iIndex, unsigned int iValue) {
    _data[iIndex][3] = normalize(iValue);
}
