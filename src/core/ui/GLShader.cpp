#include <Pch.h>
#include <glad/gl.h>
#include "GLShader.h"

GLShader::GLShader(const std::string &iCode, GLenum iType) {
    _ref = glCreateShader(iType);

    const GLchar* source = iCode.c_str();
    glShaderSource(_ref, 1, &source, 0);
}

bool GLShader::compile() const {
    glCompileShader(_ref);

    GLint compilationSuccessful = 0;
    glGetShaderiv(_ref, GL_COMPILE_STATUS, &compilationSuccessful);

    if(compilationSuccessful == GL_FALSE) {
        GLint logLength = 0;
        glGetShaderiv(_ref, GL_INFO_LOG_LENGTH, &logLength);

        GLchar* log = new GLchar[logLength];
        glGetShaderInfoLog(_ref, logLength, &logLength, log);

        LOG_ERROR("Shader compilation error");
        LOG_ERROR(log);
        delete[] log;

        glDeleteShader(_ref);
        return false;
    }
    return true;
}