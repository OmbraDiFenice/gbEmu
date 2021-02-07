#include "GLTexture.h"

#include <Pch.h>
#include <stbi/stbi_image.h>
#include <utils/GLErrorMacros.h>

GLTexture::GLTexture(const std::string& iPath) : _ref(0), _data(nullptr) {
    int bpp;
    stbi_set_flip_vertically_on_load(true);
    _data = stbi_load("test.png", &_width, &_height, &bpp, 4);
    ASSERT(_data != nullptr, "failed to load image from file");

    GLCall(glGenTextures(1, &_ref));
    GLCall(glBindTexture(GL_TEXTURE_2D, _ref));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA,
                        GL_UNSIGNED_BYTE, _data));
    unbind();

    stbi_image_free(_data);
}

GLTexture::~GLTexture() {
    // TODO: should wrap this in GLCall but as of now it triggers an infinite
    // loop on program termination because this object has longer scope than the
    // OpenGL context
    glDeleteTextures(1, &_ref);
}

void GLTexture::bind(unsigned int slot) const {
    ASSERT(slot < 32, "slot has to be less than 32");
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, _ref));
}

void GLTexture::unbind() const { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }