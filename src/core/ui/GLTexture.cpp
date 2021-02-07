#include <Pch.h>
#include "GLTexture.h"
#include <stbi/stbi_image.h>

GLTexture::GLTexture(const std::string& iPath) {
    int bpp;
    _data = stbi_load("test.png", &_width, &_height, &bpp, 4);

    glGenTextures(1, &_ref);
    glBindTexture(GL_TEXTURE_2D, _ref);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
    unbind();

    if(_data) {
        stbi_image_free(_data);
    }
}

GLTexture::~GLTexture() {
    glDeleteTextures(1, &_ref);
}

void GLTexture::bind(unsigned int slot) const {
    ASSERT(slot < 32, "slot has to be less than 32");
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, _ref);
}

void GLTexture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}