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
            case GL_UNSIGNED_INT:
                return count * sizeof(uint32_t);
        }
        return 0;
    }

    bool operator==(const VertexElement& iOther) const;
    friend std::ostream& operator<<(std::ostream& os,
                                    const VertexElement& iLayout);
};

class VertexLayout {
   public:
    VertexLayout() : _stride(0), _count(0){};
    VertexLayout(std::initializer_list<VertexElement> iElements);

    inline size_t getElementCount() const { return _elements.size(); }
    /// how many bytes a single vertex takes
    inline int getStride() const { return _stride; }
    /// how many total elements a single vertex contains
    inline int getCount() const { return _count; }
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
    int _stride;
    int _count;
};

/** Represents a vertex buffer loaded on the GPU
 *
 */
class VertexBuffer {
   public:
    virtual ~VertexBuffer() = default;

    virtual void setData(void* iData, size_t iSize) const = 0;
    virtual void bind() const                             = 0;
    virtual void unbind() const                           = 0;

    inline void setLayout(const VertexLayout& iLayout) { _layout = iLayout; }
    inline const VertexLayout& getLayout() const { return _layout; }

   protected:
    VertexLayout _layout;
};
