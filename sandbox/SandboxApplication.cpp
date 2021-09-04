#include <Pch.h>
#include <core/emu/utils.h>
#include <core/ui/opengl/GLProgram.h>
#include <core/ui/opengl/GLRenderer.h>
#include <core/ui/opengl/GLTexture.h>
#include <utils/GLErrorMacros.h>

#include "SandboxApplication.h"

SandboxApplication::SandboxApplication() {
    _renderer = std::make_unique<GLRenderer>();
}

void SandboxApplication::init() {
    _program = std::make_unique<GLProgram>();
    //_program->addShader("sandbox/compute.shader", GL_COMPUTE_SHADER);
    _program->loadShader("sandbox/vertex.shader", GL_VERTEX_SHADER);
    _program->loadShader("sandbox/fragment.shader", GL_FRAGMENT_SHADER);
    _program->link();

    _quad[0].position = glm::vec3(-0.5f, -0.5f, 0.0f);
    _quad[1].position = glm::vec3(0.5f, -0.5f, 0.0f);
    _quad[2].position = glm::vec3(0.5f, 0.5f, 0.0f);
    _quad[3].position = glm::vec3(-0.5f, 0.5f, 0.0f);

    // unsigned char* tileMapPatterns = loadData("tileDataTable_8800.DMP");

    int width = 8 / 8, height = 512 / 8;
    int slot = 1;
    GLTexture texture(nullptr, width, height, slot, 1, 4);
    GLCall(glBindImageTexture(slot, texture.getRef(), 0, GL_FALSE, 0,
                              GL_WRITE_ONLY, GL_RGBA8));

    // delete[] tileMapPatterns;

    _program->bind();
    texture.bind();
    _quad.bind();
}
void SandboxApplication::drawScreen() { _renderer->draw(_quad.getBuffer()); }
