#include <Pch.h>
#include <glad/gl.h>
#include <core/ui/GLProgram.h>
#include <core/ui/GLShader.h>
#include <utils/GLErrorMacros.h>

GLProgram::GLProgram() {
    GLCall(_ref = glCreateProgram());
}

GLProgram::~GLProgram() {
    // TODO: should wrap this in GLCall but as of now it triggers an infinite
    // loop on program termination because this object has longer scope than the
    // OpenGL context

    glDeleteProgram(_ref);
    for(auto& shader : _shaderList) {
        glDeleteShader(static_cast<const GLShader*>(shader.get())->getRef());
    }
}

void GLProgram::addShader(const std::string& iSrc, GLenum iType) {
    std::shared_ptr<const Shader> shader = std::make_shared<const GLShader>(iSrc, iType);
    Program::addShader(shader);
}

bool GLProgram::link() const {
    for(auto& shader : _shaderList) {
        if(!shader->compile()) return false;
        GLCall(glAttachShader(_ref, static_cast<const GLShader*>(shader.get())->getRef()));
    }

    GLCall(glLinkProgram(_ref));

    GLint linkingSuccessful = 0;
    GLCall(glGetProgramiv(_ref, GL_LINK_STATUS, &linkingSuccessful));

    if(linkingSuccessful == GL_FALSE) {
        GLint logLength = 0;
        GLCall(glGetShaderiv(_ref, GL_INFO_LOG_LENGTH, &logLength));

        GLchar* log = new GLchar[logLength];
        GLCall(glGetProgramInfoLog(_ref, logLength, &logLength, log));

        LOG_ERROR("Shader link error");
        LOG_ERROR(log);
        delete[] log;

        GLCall(glDeleteProgram(_ref));
        for(auto& shader : _shaderList) {
            GLCall(glDeleteShader(static_cast<const GLShader*>(shader.get())->getRef()));
        }
        return false;
    }

    for(auto& shader : _shaderList) {
        GLCall(glDetachShader(_ref, static_cast<const GLShader*>(shader.get())->getRef()));
    }

    return true;
}

void GLProgram::bind() const {
    GLCall(glUseProgram(_ref));
}

void GLProgram::unbind() const {
    GLCall(glUseProgram(0));
}

void GLProgram::setUniform(const std::string& iName, int iValue) {
    GLCall(glUniform1i(glGetUniformLocation(_ref, iName.c_str()), iValue));
}
