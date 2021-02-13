#include <Pch.h>

#include "GLTexture.h"

#include <stbi/stbi_image.h>
#include <utils/GLErrorMacros.h>

GLTexture::GLTexture(const std::string &iPath) : Texture(iPath) {
    loadFromFile(iPath);
}

GLTexture::GLTexture(unsigned char *iData, int iWidth, int iHeight,
                     unsigned int channels)
    : Texture(iData, iWidth, iHeight), _ref(0) {
    initTexture(channels);
}

GLTexture::~GLTexture() {
    // TODO: should wrap this in GLCall but as of now it triggers an infinite
    // loop on program termination because this object has longer scope than the
    // OpenGL context
    glDeleteTextures(1, &_ref);
}

void GLTexture::initTexture(unsigned int channels) {
    GLCall(glGenTextures(1, &_ref));
    GLCall(glBindTexture(GL_TEXTURE_2D, _ref));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    GLenum internalFormat = GetGlFormatFromChannels(channels);
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, _data));
    unbind();
}

GLenum GLTexture::GetGlFormatFromChannels(unsigned int channels) {
    switch (channels) {
        case 1:
            return GL_R8;
        case 2:
            return GL_RG8;
        case 3:
            return GL_RGB8;
        case 4:
            return GL_RGBA8;
        default:
            return GL_RGBA8;
    }
}

void GLTexture::loadFromFile(const std::string &iPath) {
    int bpp;
    stbi_set_flip_vertically_on_load(true);
    _data = stbi_load("test.png", &_width, &_height, &bpp, 4);
    ASSERT(_data != nullptr, "failed to load image from file");

    initTexture(4);

    stbi_image_free(_data);
}

void GLTexture::bind(unsigned int slot) const {
    ASSERT(slot < 32, "slot has to be less than 32");
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, GLTexture::_ref));
}

void GLTexture::unbind() const { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }
