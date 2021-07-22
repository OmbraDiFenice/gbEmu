#pragma once
#include <core/ui/Program.h>
#include <core/ui/Shader.h>
#include <glad/gl.h>

class GLProgram : public Program {
   public:
    GLProgram();
    ~GLProgram() override;

    void addShader(const std::string& iSrc, unsigned int iType) override;
    void loadShader(const std::string& iPath, unsigned int iType) override;
    GLuint getRef() const { return _ref; }
    bool link() const override;
    void bind() const override;
    void unbind() const override;

    void setUniform(const std::string& iName, int iValue) const override;
    void setUniform(const std::string& iName, float iValue) const override;
    void setUniform(const std::string& iName, size_t iSize,
                    int* iValues) const override;
    void setUniformMatrix3(const std::string& iName,
                           const float* iData) const override;
    void setUniformMatrix4(const std::string& iName,
                           const float* iData) const override;

   private:
    GLuint _ref;
};
