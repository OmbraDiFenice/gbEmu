#pragma once
#include <core/ui/Program.h>
#include <glad/gl.h>

class GLProgram : public Program {
   public:
    GLProgram();
    ~GLProgram() override;

    void addShader(const std::string& iSrc, GLenum iType);
    [[nodiscard]] GLuint getRef() const { return _ref; }
    bool link() const override;
    void bind() const override;
    void unbind() const override;

   private:
    GLuint _ref;
};
