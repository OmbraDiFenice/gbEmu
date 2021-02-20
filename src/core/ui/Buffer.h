#pragma once

#include <glad/gl.h>
#include <utility>
#include <vector>

struct VertexElement {
    int size;
    int type;

    VertexElement(int iSize, int iType) : size(iSize), type(iType){};

    size_t bytes() const {
        switch (type) {
            case GL_FLOAT:
                return size * sizeof(float);
        }
        return 0;
    }
};

class VertexLayout {
   public:
    VertexLayout() : stride(0), values(0){};
    VertexLayout(std::initializer_list<VertexElement> iElements);

    inline size_t getElementCount() const { return _elements.size(); }
    inline int getStride() const { return stride; }
    inline int getValues() const { return values; }
    inline const std::vector<VertexElement>& getElements() const {
        return _elements;
    }

   private:
    void computeStride();

   private:
    std::vector<VertexElement> _elements;
    int stride;
    int values;
};

class Buffer {
   public:
    inline void setIndexBuffer(std::vector<unsigned int> iIndexBuffer) {
        _ib = std::move(iIndexBuffer);
    }
    inline void setVertexBuffer(float* iVertexBuffer, size_t iBufferSize) {
        _vb     = iVertexBuffer;
        _vbSize = iBufferSize;
    }
    inline void setVertexLayout(const VertexLayout& iLayout) {
        _layout = iLayout;
    }
    inline float* getVertexBuffer() { return _vb; }

    virtual void bind() const = 0;

    void setVertexElement(unsigned int iVertex, unsigned int iElement,
                          unsigned int iElementOffset, float iValue);

   protected:
    std::vector<unsigned int> _ib;
    float* _vb;
    size_t _vbSize;
    VertexLayout _layout;
};
