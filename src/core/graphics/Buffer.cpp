#include <Pch.h>

#include <core/graphics/Buffer.h>

VertexLayout::VertexLayout(std::initializer_list<VertexElement> iElements)
    : _elements(iElements) {
    computeStride();
};

void VertexLayout::computeStride() {
    _stride = 0;
    _count  = 0;
    for (const auto& element : _elements) {
        _stride += element.size();
        _count += element.count;
    }
}

bool VertexLayout::operator==(const VertexLayout& iOther) const {
    return _elements == iOther._elements;
}

bool VertexElement::operator==(const VertexElement& iOther) const {
    return count == iOther.count && type == iOther.type;
}

std::ostream& operator<<(std::ostream& os, const VertexLayout& iLayout) {
    os << "[ ";
    for (const auto& element : iLayout._elements) {
        os << element << " ";
    }
    os << "] _stride: " << iLayout._stride << " values: " << iLayout._count;
    return os;
}

std::ostream& operator<<(std::ostream& os, const VertexElement& iElement) {
    return os << "(type: " << iElement.type << ", count: " << iElement.count
              << ")";
}
