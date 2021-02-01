#include <Pch.h>
#include <glad/gl.h>
#include <core/ui/GLProgram.h>
#include <core/ui/GLShader.h>

GLProgram::GLProgram() {
    _ref = glCreateProgram();
}

GLProgram::~GLProgram() {
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
        glAttachShader(_ref, static_cast<const GLShader*>(shader.get())->getRef());
    }

    glLinkProgram(_ref);

    GLint linkingSuccessful = 0;
    glGetProgramiv(_ref, GL_LINK_STATUS, &linkingSuccessful);

    if(linkingSuccessful == GL_FALSE) {
        GLint logLength = 0;
        glGetShaderiv(_ref, GL_INFO_LOG_LENGTH, &logLength);

        GLchar* log = new GLchar[logLength];
        glGetProgramInfoLog(_ref, logLength, &logLength, log);

        LOG_ERROR("Shader link error");
        LOG_ERROR(log);
        delete[] log;

        glDeleteProgram(_ref);
        for(auto& shader : _shaderList) {
            glDeleteShader(static_cast<const GLShader*>(shader.get())->getRef());
        }
        return false;
    }

    for(auto& shader : _shaderList) {
        glDetachShader(_ref, static_cast<const GLShader*>(shader.get())->getRef());
    }

    return true;
}

void GLProgram::bind() const {
    glUseProgram(_ref);
}

void GLProgram::unbind() const {
    glUseProgram(0);
}
