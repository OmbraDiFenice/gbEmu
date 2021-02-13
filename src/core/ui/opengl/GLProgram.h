#pragma once
#include <core/ui/Program.h>
#include <core/ui/Shader.h>
#include <glad/gl.h>

class GLProgram : public Program {
   public:
    GLProgram();
    ~GLProgram() override;

    void addShader(const std::string& iSrc, GLenum iType);
    void loadShader(const std::string& iPath, GLenum iType);
    GLuint getRef() const { return _ref; }
    bool link() const override;
    void bind() const override;
    void unbind() const override;

    void setUniform(const std::string& iName, int iValue) override;
    void setUniformMatrix4(const std::string& iName, const float* iData,
                           int iCount) const;

   private:
    GLuint _ref;
};