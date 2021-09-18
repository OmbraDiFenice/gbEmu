#include <Pch.h>

#include <core/graphics/opengl/GLShader.h>

#include <glad/gl.h>
#include <utils/GLErrorMacros.h>

bool GLShader::compile() const {
    GLCall(_ref = glCreateShader(_type));
    const GLchar* source = _source.c_str();
    GLCall(glShaderSource(_ref, 1, &source, 0));

    GLCall(glCompileShader(_ref));

    GLint compilationSuccessful = 0;
    GLCall(glGetShaderiv(_ref, GL_COMPILE_STATUS, &compilationSuccessful));

    if (compilationSuccessful == GL_FALSE) {
        LOG_ERROR("Shader compilation error");

        GLint logLength = 0;
        GLCall(glGetShaderiv(_ref, GL_INFO_LOG_LENGTH, &logLength));

        if (logLength != 0) {
            auto* log = new GLchar[logLength];
            GLCall(glGetShaderInfoLog(_ref, logLength, &logLength, log));

            LOG_ERROR(log);
            delete[] log;
        }

        glDeleteShader(_ref);
        return false;
    }
    return true;
}
