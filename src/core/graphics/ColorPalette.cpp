#include <Pch.h>

#include <core/graphics/ColorPalette.h>

ColorPalette::ColorPalette(const glm::mat4& data) {
    _data = data;
    normalize();
}

ColorPalette::ColorPalette(const glm::mat4&& data) {
    _data = std::move(data);
    normalize();
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

float ColorPalette::normalize(unsigned int iValue) { return iValue / 255.0f; }

void ColorPalette::normalize() { _data /= 255.0f; }
