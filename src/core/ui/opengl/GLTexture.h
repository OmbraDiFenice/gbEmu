#pragma once
#include "core/ui/Texture.h"
#include "glad/gl.h"

class GLTexture : public Texture {
   public:
    GLTexture(const std::string& iPath, unsigned int iTextureSlot,
              unsigned int iInputChannels, unsigned int iOutputChannels);
    GLTexture(std::shared_ptr<unsigned char[]> iData, int iWidth, int iHeight,
              unsigned int iTextureSlot, unsigned int iInputChannels,
              unsigned int iOutputChannels);
    ~GLTexture() override;

    void bind() const override;
    void unbind() const override;

    void associateToWritableBuffer(GLuint iWritableBufferRef) const override;

   private:
    void loadFromFile(const std::string& iPath);
    void initTexture(unsigned int iInputChannels, unsigned int iOutputChannels);

    static GLenum GetGlFormatFromInputChannels(unsigned int iInputChannels);
    static GLenum GetGlFormatFromOutputChannels(unsigned int iOutputChannels);

   private:
    GLuint _ref = 0;
    GLenum _internalFormat;
};
