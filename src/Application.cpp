#include "Application.h"
#include <glad/gl.h>

void Application::run() {
    LOG_DBG("start app");

    WindowProp prop;
    prop.width = prop.height = 640;
    Window* window = _windowManager->createWindow(prop);
    _components.push_back(window);

    window->setEventCallback(BIND_FN(Application::onEvent));

    while (keepRunning()) {

        // data setup
        unsigned int vertexArray;
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);

        unsigned int vertexBuffer;
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        // clang-format off
        float vertices[4 * 3] = {
            -0.5f, -0.5f,  0.0f,
            0.5f, -0.5f,  0.0f,
            0.5f,  0.5f,  0.0f,
            -0.5f,  0.5f,  0.0f
        };
        // clang-format on

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        unsigned int indexBuffer;
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

        unsigned int indices[6] = {0, 1, 2, 2, 3, 0};
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                     GL_STATIC_DRAW);

        // drawing
        glClearColor(0.15f, 0.15f, 0.15f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // glBindVertexArray(id); // bind updated vertex array
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        for (auto c : _components) {
            c->update();
        }
    }

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

void Application::onKeyEvent(KeyEvent& e) {
    TRACE_CALL
}

void Application::onMouseEvent(MouseEvent& e) {
    TRACE_CALL
}

bool Application::keepRunning() {
    return WindowManager::GetWindowCount() > 0;
}
