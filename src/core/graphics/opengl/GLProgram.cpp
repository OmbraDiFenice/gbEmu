#include <Pch.h>

#include <core/graphics/opengl/GLProgram.h>

#include <core/graphics/opengl/GLShader.h>
#include <glad/gl.h>
#include <utils/GLErrorMacros.h>

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
    GLCall(_ref = glCreateProgram());

    for (auto& shader : _shaderList) {
        if (!shader->compile()) return false;
        GLCall(glAttachShader(
            _ref, static_cast<const GLShader*>(shader.get())->getRef()));
    }

    GLCall(glLinkProgram(_ref));

    GLint linkingSuccessful = 0;
    GLCall(glGetProgramiv(_ref, GL_LINK_STATUS, &linkingSuccessful));

    if (linkingSuccessful == GL_FALSE) {
        LOG_ERROR("Shader link error");

        GLint logLength = 0;
        GLCall(glGetShaderiv(_ref, GL_INFO_LOG_LENGTH, &logLength));

        if (logLength != 0) {
            GLchar* log = new GLchar[logLength];
            GLCall(glGetProgramInfoLog(_ref, logLength, &logLength, log));

            LOG_ERROR(log);
            delete[] log;
        }

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

void GLProgram::setUniform(const std::string& iName, int iValue) const {
    GLCall(GLint location = glGetUniformLocation(_ref, iName.c_str()));
    if (location == -1) {
        LOG_WARN("uniform " << iName << " not found in program");
    } else {
        GLCall(glUniform1i(location, iValue));
    }
}

void GLProgram::setUniform(const std::string& iName, size_t iSize,
                           int* iValues) const {
    GLCall(GLint location = glGetUniformLocation(_ref, iName.c_str()));
    if(location == -1) {
        LOG_WARN("uniform " << iName << " not found in program");
    } else {
        GLCall(glUniform1iv(location, iSize, iValues));
    }
}

void GLProgram::setUniform(const std::string& iName, float iValue) const {
    GLCall(GLint location = glGetUniformLocation(_ref, iName.c_str()));
    if (location == -1) {
        LOG_WARN("uniform " << iName << " not found in program");
    } else {
        GLCall(glUniform1f(location, iValue));
    }
}

void GLProgram::setUniformMatrix3(const std::string& iName,
                                  const float* iData) const {
    GLCall(GLint location = glGetUniformLocation(_ref, iName.c_str()));
    if (location == -1) {
        LOG_WARN("uniform " << iName << " not found in program");
    } else {
        GLCall(glUniformMatrix3fv(location, 1, GL_FALSE, iData));
    }
}

void GLProgram::setUniformMatrix4(const std::string& iName,
                                  const float* iData) const {
    GLCall(GLint location = glGetUniformLocation(_ref, iName.c_str()));
    if (location == -1) {
        LOG_WARN("uniform " << iName << " not found in program");
    } else {
        GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, iData));
    }
}

void GLProgram::execute(unsigned int iWorkGroupX, unsigned int iWorkGroupY,
                        unsigned int iWorkGroupZ) const {
    GLCall(glDispatchCompute(iWorkGroupX, iWorkGroupY, iWorkGroupZ));
}
