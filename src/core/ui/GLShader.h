#pragma once
#include <string>
#include <core/ui/Shader.h>
#include <glad/gl.h>

class GLShader : public Shader {
   public:
    GLShader(const std::string& iCode, GLenum iType);

    GLuint getRef() const { return _ref; }
    bool compile() const override;

   private:
    GLuint _ref;
};
