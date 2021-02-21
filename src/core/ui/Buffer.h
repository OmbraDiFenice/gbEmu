#pragma once

#include <glad/gl.h>
#include <utility>
#include <vector>

struct VertexElement {
    int count;
    int type;

    VertexElement(int iCount, int iType) : count(iCount), type(iType){};

    size_t size() const {
        switch (type) {
            case GL_FLOAT:
                return count * sizeof(float);
        }
        return 0;
    }

    bool operator==(const VertexElement& iOther) const;
    friend std::ostream& operator<<(std::ostream& os,
                                    const VertexElement& iLayout);
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

    bool operator==(const VertexLayout& iOther) const;
    friend std::ostream& operator<<(std::ostream& os,
                                    const VertexLayout& iLayout);

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
    virtual inline void setVertexBuffer(float* iVertexBuffer,
                                        size_t iBufferCount) {
        _vb      = iVertexBuffer;
        _vbCount = iBufferCount;
    }
    virtual inline void setVertexLayout(const VertexLayout& iLayout) {
        _layout = iLayout;
    }
    inline float* getVertexBuffer() const { return _vb; }
    inline size_t getVertexBufferSize() const {
        return _vbCount * sizeof(float);
    }
    inline size_t getVertexBufferCount() const { return _vbCount; }
    inline const std::vector<unsigned int>& getIndexBuffer() const {
        return _ib;
    }
    inline const VertexLayout& getLayout() const { return _layout; }

    virtual void bind() const = 0;

    void setVertexElement(unsigned int iVertex, unsigned int iElement,
                          unsigned int iElementOffset, float iValue);

   protected:
    std::vector<unsigned int> _ib;
    float* _vb;
    size_t _vbCount;
    VertexLayout _layout;
};
