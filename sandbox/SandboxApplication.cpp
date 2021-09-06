#include <Pch.h>
#include <core/emu/utils.h>
#include <core/ui/opengl/GLProgram.h>
#include <core/ui/opengl/GLRenderer.h>
#include <core/ui/opengl/GLShaderStorageBuffer.h>
#include <core/ui/opengl/GLTexture.h>
#include <core/ui/opengl/Synchronization.h>
#include <utils/GLErrorMacros.h>

#include "SandboxApplication.h"

SandboxApplication::SandboxApplication()
    : _colorPalette({{0xD0, 0xE0, 0xF0, 0xFF},
                     {0x98, 0x98, 0x98, 0xFF},
                     {0x68, 0x68, 0x68, 0xFF},
                     {0x38, 0x38, 0x38, 0xFF}}) {
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

    _texture = std::make_unique<GLTexture>(nullptr, 8, 8 * 256, 1, 4, 4);

    unsigned char* tileMapPatterns = loadData("tileDataTable_8800.DMP");
    _compressedTileData            = std::make_unique<GLShaderStorageBuffer>();
    _compressedTileData->uploadData(tileMapPatterns, 4096, GL_STATIC_READ);
    delete[] tileMapPatterns;
}

void SandboxApplication::drawScreen() {
    _computeProgram->bind();
    _compressedTileData->bind();
    int textureRef       = _texture->getTextureSlot();
    int textureBufferRef = 0;
    _computeProgram->setUniform("u_ImageOutput", textureBufferRef);
    _computeProgram->setUniformMatrix4("u_Palette", _colorPalette);

    _texture->associateToWritableBuffer(0);

    _computeProgram->execute(256, 1, 1);

    SynchronizeOn(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    _renderProgram->bind();
    _renderProgram->setUniform("u_Texture", textureRef);
    _texture->bind();
    _quad.bind();
    _renderer->draw(_quad.getBuffer());
}
