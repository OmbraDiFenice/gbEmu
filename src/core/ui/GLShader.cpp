#include <Pch.h>
#include <glad/gl.h>
#include "GLShader.h"
#include <utils/GLErrorMacros.h>

GLShader::GLShader(const std::string &iCode, GLenum iType) {
    GLCall(_ref = glCreateShader(iType));

    const GLchar* source = iCode.c_str();
    GLCall(glShaderSource(_ref, 1, &source, 0));
}

bool GLShader::compile() const {
    GLCall(glCompileShader(_ref));

    GLint compilationSuccessful = 0;
    GLCall(glGetShaderiv(_ref, GL_COMPILE_STATUS, &compilationSuccessful));

    if(compilationSuccessful == GL_FALSE) {
        GLint logLength = 0;
        GLCall(glGetShaderiv(_ref, GL_INFO_LOG_LENGTH, &logLength));

        GLchar* log = new GLchar[logLength];
        GLCall(glGetShaderInfoLog(_ref, logLength, &logLength, log));

        LOG_ERROR("Shader compilation error");
        LOG_ERROR(log);
        delete[] log;

        glDeleteShader(_ref);
        return false;
    }
    return true;
}