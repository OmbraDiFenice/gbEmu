#pragma once
#include "glad/gl.h"

class GLTexture {
   public:
    GLTexture(const std::string& iPath);
    GLTexture(unsigned char* iData, int iWidth, int iHeight);
    virtual ~GLTexture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

   protected:
    void initTexture();

   private:
    GLuint _ref;
    unsigned char* _data;
    int _width;
    int _height;
};
