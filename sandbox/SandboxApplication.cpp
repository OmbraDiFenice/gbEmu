#include <Pch.h>
#include <core/emu/utils.h>
#include <core/ui/opengl/GLProgram.h>
#include <core/ui/opengl/GLRenderer.h>
#include <core/ui/opengl/GLTexture.h>
#include <utils/GLErrorMacros.h>

#include "SandboxApplication.h"

SandboxApplication::SandboxApplication() {
    _renderer       = std::make_unique<GLRenderer>();
    _renderProgram  = std::make_unique<GLProgram>();
    _computeProgram = std::make_unique<GLProgram>();
}

void SandboxApplication::init() {
    _renderProgram->loadShader("sandbox/vertex.shader", GL_VERTEX_SHADER);
    _renderProgram->loadShader("sandbox/fragment.shader", GL_FRAGMENT_SHADER);
    _renderProgram->link();

    _computeProgram->loadShader("sandbox/compute.shader", GL_COMPUTE_SHADER);
    _computeProgram->link();

    _quad[0].position = glm::vec3(-0.5f, -0.5f, 0.0f);
    _quad[1].position = glm::vec3(0.5f, -0.5f, 0.0f);
    _quad[2].position = glm::vec3(0.5f, 0.5f, 0.0f);
    _quad[3].position = glm::vec3(-0.5f, 0.5f, 0.0f);

    // unsigned char* tileMapPatterns = loadData("tileDataTable_8800.DMP");

    _texture = std::make_unique<GLTexture>(nullptr, 8, 512, 1, 4, 4);

    // delete[] tileMapPatterns;
}
void SandboxApplication::drawScreen() {
    _computeProgram->bind();
    int textureRef       = _texture->getTextureSlot();
    int textureBufferRef = 0;
    _computeProgram->setUniform("img_output", textureBufferRef);

    GLCall(glBindImageTexture(textureBufferRef, textureRef, 0, GL_FALSE, 0,
                              GL_WRITE_ONLY, GL_RGBA8));

    GLCall(glDispatchCompute(static_cast<GLuint>(_texture->getWidth()),
                             static_cast<GLuint>(_texture->getHeight()), 1));

    GLCall(glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));

    _renderProgram->bind();
    _renderProgram->setUniform("u_Texture", textureRef);
    _texture->bind();
    _quad.bind();
    _renderer->draw(_quad.getBuffer());
}
