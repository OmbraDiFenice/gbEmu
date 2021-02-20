#include <Pch.h>

#include "Buffer.h"

void Buffer::setVertexElement(unsigned int iVertex, unsigned int iElement,
                              unsigned int iElementOffset, float iValue) {
    ASSERT(iVertex < _vbSize, "invalid vertex selected");
    int vertexIndex = iVertex * _layout.getValues();

    ASSERT(iElement < _layout.getElements().size(), "invalid element selected");
    int elementIndex = 0;
    if (iElement > 0) {
        ASSERT(iElementOffset < _layout.getElements().at(iElement - 1).size,
               "invalid element offset");
        elementIndex = iElement * _layout.getElements().at(iElement - 1).size;
    }

    _vb[vertexIndex + elementIndex + iElementOffset] = iValue;
}

VertexLayout::VertexLayout(std::initializer_list<VertexElement> iElements)
    : _elements(iElements) {
    computeStride();
};

void VertexLayout::computeStride() {
    stride = 0;
    values = 0;
    for (const auto& element : _elements) {
        stride += element.bytes();
        values += element.size;
    }
}
