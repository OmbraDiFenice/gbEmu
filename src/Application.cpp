#include "Application.h"

#include <GLWindowManager.h>
#include <Macros.h>

Application::Application() { GLWindowManager::Init(); }
Application::~Application() { GLWindowManager::Destroy(); }

void Application::run() {
    LOG_DBG("start app");

    GLWindow window = GLWindowManager::CreateWindow();
    _components.push_back(&window);

    window.setEventCallback(BIND_FN(Application::onEvent));

    while (GLWindowManager::GetWindowCount() > 0) {
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