#pragma once

#include <Application.h>
#include <core/ui/Program.h>
#include <core/ui/Renderer.h>
#include <core/ui/Texture.h>
#include <glm/vec3.hpp>
#include "Quad.h"

struct Vertex {
    glm::vec3 position;

    static const VertexLayout& ToLayout() {
        // clang-format off
        static VertexLayout layout{
            {3, GL_FLOAT},
        };
        // clang-format on
        return layout;
    }
};

class SandboxApplication : public Application {
   public:
    SandboxApplication();

   private:
    void init() override;
    void drawScreen() override;

   private:
    Quad<Vertex> _quad;
    std::unique_ptr<Program> _renderProgram;
    std::unique_ptr<Program> _computeProgram;
    std::unique_ptr<Renderer> _renderer;
    std::unique_ptr<Texture> _texture;
    GLuint _compressedDataRef;
};
