#include <Pch.h>

#include "GLTexture.h"

#include <stbi/stbi_image.h>
#include <utils/GLErrorMacros.h>

#include <utility>

GLTexture::GLTexture(const std::string& iPath, unsigned int iTextureSlot,
                     unsigned int iInputChannels, unsigned int iOutputChannels)
    : Texture(iPath, iTextureSlot) {
    loadFromFile(iPath);
    initTexture(iInputChannels, iOutputChannels);
}

GLTexture::GLTexture(std::shared_ptr<unsigned char[]> iData, int iWidth,
                     int iHeight, unsigned int iTextureSlot,
                     unsigned int iInputChannels, unsigned int iOutputChannels)
    : Texture(std::move(iData), iWidth, iHeight, iTextureSlot), _ref(0) {
    initTexture(iInputChannels, iOutputChannels);
}

GLTexture::~GLTexture() {
    // TODO: should wrap this in GLCall but as of now it triggers an infinite
    // loop on program termination because this object has longer scope than the
    // OpenGL context
    glDeleteTextures(1, &_ref);
}

void GLTexture::initTexture(unsigned int iInputChannels,
                            unsigned int iOutputChannels) {
    GLCall(glGenTextures(1, &_ref));
    bind();

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    GLenum internalFormat = GetGlFormatFromInputChannels(iInputChannels);
    GLenum format         = GetGlFormatFromOutputChannels(iOutputChannels);
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0,
                        format, GL_UNSIGNED_BYTE, _data.get()));
    unbind();
}

GLenum GLTexture::GetGlFormatFromInputChannels(unsigned int iInputChannels) {
    switch (iInputChannels) {
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

GLenum GLTexture::GetGlFormatFromOutputChannels(unsigned int iOutputChannels) {
    switch (iOutputChannels) {
        case 1:
            return GL_RED;
        case 4:
            return GL_RGBA;
        default:
            return GL_RGBA;
    }
}

void GLTexture::loadFromFile(const std::string& iPath) {
    int bpp;
    stbi_set_flip_vertically_on_load(true);
    _data.reset(stbi_load("test.png", &_width, &_height, &bpp, 4));
    ASSERT(_data != nullptr, "failed to load image from file");
}

void GLTexture::bind() const {
    GLCall(glActiveTexture(GL_TEXTURE0 + _textureSlot));
    GLCall(glBindTexture(GL_TEXTURE_2D, GLTexture::_ref));
}

void GLTexture::unbind() const { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }
