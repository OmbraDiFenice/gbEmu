#include <Pch.h>

#include <core/ui/opengl/GLProgram.h>

#include <core/ui/opengl/GLShader.h>
#include <glad/gl.h>
#include <utils/GLErrorMacros.h>

GLProgram::GLProgram() { GLCall(_ref = glCreateProgram()); }

GLProgram::~GLProgram() {
    // TODO: should wrap this in GLCall but as of now it triggers an infinite
    // loop on program termination because this object has longer scope than the
    // OpenGL context

    glDeleteProgram(_ref);
    for (auto& shader : _shaderList) {
        glDeleteShader(static_cast<const GLShader*>(shader.get())->getRef());
    }
}

void GLProgram::addShader(const std::string& iSrc, GLenum iType) {
    std::shared_ptr<const Shader> shader =
        std::make_shared<const GLShader>(iSrc, iType);
    Program::addShader(shader);
}

void GLProgram::loadShader(const std::string& iFilePath, GLenum iType) {
    std::ifstream in(iFilePath, std::ios::in);
    std::string shaderSrc = std::string((std::istreambuf_iterator<char>(in)),
                                        std::istreambuf_iterator<char>());
    addShader(shaderSrc, iType);
}

bool GLProgram::link() const {
    for (auto& shader : _shaderList) {
        if (!shader->compile()) return false;
        GLCall(glAttachShader(
            _ref, static_cast<const GLShader*>(shader.get())->getRef()));
    }

    GLCall(glLinkProgram(_ref));

    GLint linkingSuccessful = 0;
    GLCall(glGetProgramiv(_ref, GL_LINK_STATUS, &linkingSuccessful));

    if (linkingSuccessful == GL_FALSE) {
        GLint logLength = 0;
        GLCall(glGetShaderiv(_ref, GL_INFO_LOG_LENGTH, &logLength));

        GLchar* log = new GLchar[logLength];
        GLCall(glGetProgramInfoLog(_ref, logLength, &logLength, log));

        LOG_ERROR("Shader link error");
        LOG_ERROR(log);
        delete[] log;

        GLCall(glDeleteProgram(_ref));
        for (auto& shader : _shaderList) {
            GLCall(glDeleteShader(
                static_cast<const GLShader*>(shader.get())->getRef()));
        }
        return false;
    }

    for (auto& shader : _shaderList) {
        GLCall(glDetachShader(
            _ref, static_cast<const GLShader*>(shader.get())->getRef()));
    }

    return true;
}

void GLProgram::bind() const { GLCall(glUseProgram(_ref)); }

void GLProgram::unbind() const { GLCall(glUseProgram(0)); }

void GLProgram::setUniform(const std::string& iName, int iValue) {
    GLCall(GLint location = glGetUniformLocation(_ref, iName.c_str()));
    if (location == -1) {
        LOG_WARN("uniform " << iName << " not found in program");
    } else {
        GLCall(glUniform1i(location, iValue));
    }
}

void GLProgram::setUniformMatrix4(const std::string& iName, const float* iData,
                                  const int iCount) const {
    GLCall(GLint location = glGetUniformLocation(_ref, iName.c_str()));
    if (location == -1) {
        LOG_WARN("uniform " << iName << " not found in program");
    } else {
        GLCall(glUniformMatrix4fv(location, iCount, GL_FALSE, iData));
    }
}
