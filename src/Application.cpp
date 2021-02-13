#include <Pch.h>

#include "Application.h"

#include <core/TileMapAdapter.h>
#include <core/emu/Video.h>
#include <core/emu/utils.h>
#include <core/ui/GLProgram.h>
#include <core/ui/GLShader.h>
#include <core/ui/Texture.h>
#include <glad/gl.h>
#include <utils/GLErrorMacros.h>

void Application::run() {
    LOG_DBG("start app");

    WindowProp prop;
    prop.width = prop.height = 640;

    Window* window = _windowManager->createWindow(prop);
    _components.push_back(window);

    window->setEventCallback(BIND_FN(Application::onEvent));

    unsigned char* tileMap = loadData("tileMap_8800.DMP");

    Video video;
    video.decodeTileMap(tileMap);

    // clang-format off
    float vertices[4 * 5] = {
        -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f, 0.0f, 1.0f
    };
    // clang-format on

    unsigned int vertexArray;
    GLCall(glGenVertexArrays(1, &vertexArray));

    unsigned int vertexBuffer;
    GLCall(glGenBuffers(1, &vertexBuffer));

    unsigned int indices[6] = {0, 1, 2, 2, 3, 0};
    unsigned int indexBuffer;
    GLCall(glGenBuffers(1, &indexBuffer));

    GLProgram program;
    program.loadShader("vertex.shader", GL_VERTEX_SHADER);
    program.loadShader("fragment.shader", GL_FRAGMENT_SHADER);
    program.link();
    program.bind();

    TileMapAdapter tileMapAdapter;
    const Texture& texture = tileMapAdapter.toTexture(video.tileMap[119]);
    texture.bind(0);
    program.setUniform("u_Texture", 0);

    while (keepRunning()) {
        // data setup

        GLCall(glBindVertexArray(vertexArray));

        GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                            GL_STATIC_DRAW));

        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                     5 * sizeof(float), nullptr));
        GLCall(glEnableVertexAttribArray(1));
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                                     5 * sizeof(float),
                                     (const void*)(3 * sizeof(float))));

        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                            GL_STATIC_DRAW));

        // drawing
        GLCall(glClearColor(0.15f, 0.15f, 0.15f, 1));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        for (auto c : _components) {
            c->update();
        }
    }

    delete[] tileMap;
    LOG_DBG("end app");
}

void Application::onEvent(Event& e) {
    EventDispatcher dispatcher;
    dispatcher.dispatch<KeyEvent>(BIND_FN(Application::onKeyEvent), e);
    dispatcher.dispatch<MouseEvent>(BIND_FN(Application::onMouseEvent), e);

    for (auto c : _components) {
        c->onEvent(e);
    }
}

void Application::onKeyEvent(KeyEvent& e) { TRACE_CALL }

void Application::onMouseEvent(MouseEvent& e) { TRACE_CALL }

bool Application::keepRunning() { return WindowManager::GetWindowCount() > 0; }
