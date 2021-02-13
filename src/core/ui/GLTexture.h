#pragma once
#include "Texture.h"
#include "glad/gl.h"

class GLTexture : public Texture {
   public:
    explicit GLTexture(const std::string& iPath);
    GLTexture(unsigned char* iData, int iWidth, int iHeight,
              unsigned int channels = 4);
    ~GLTexture() override;

    void bind(unsigned int slot) const override;
    void unbind() const override;

   private:
    void loadFromFile(const std::string& iPath);
    void initTexture(unsigned int channels);

    static GLenum GetGlFormatFromChannels(unsigned int channels);

   private:
    GLuint _ref = 0;
};
