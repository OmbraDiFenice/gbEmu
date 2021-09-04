#pragma once
#include <core/ui/Shader.h>
#include <glad/gl.h>
#include <string>

class GLShader : public Shader {
   public:
    GLShader(const std::string& iCode, GLenum iType)
        : _source(iCode), _type(iType){};

    GLuint getRef() const { return _ref; }
    bool compile() const override;

   private:
    mutable GLuint _ref;
    GLenum _type;
    std::string _source;
};
