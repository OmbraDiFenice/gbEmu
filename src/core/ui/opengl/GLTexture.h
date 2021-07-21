#pragma once
#include "core/ui/Texture.h"
#include "glad/gl.h"

class GLTexture : public Texture {
   public:
    explicit GLTexture(const std::string& iPath, unsigned int iChannels,
                       unsigned int iTextureSlot);
    GLTexture(std::shared_ptr<unsigned char[]> iData, int iWidth, int iHeight,
              unsigned int iTextureSlot, unsigned int channels);
    ~GLTexture() override;

    void bind() const override;
    void unbind() const override;

   private:
    void loadFromFile(const std::string& iPath);
    void initTexture(unsigned int channels);

    static GLenum GetGlFormatFromChannels(unsigned int channels);

   private:
    GLuint _ref = 0;
};
