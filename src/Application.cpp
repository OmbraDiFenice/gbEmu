#include <Pch.h>

#include <Application.h>

void Application::run() {
    LOG_DBG("start app");

    Window* window = createWindow();
    _components.push_back(window);

    window->setEventCallback(BIND_FN(Application::onEvent));

    init();

    while (keepRunning()) {
        update();
        drawScreen();

        for (auto c : _components) {
            c->update();
        }
    }

    LOG_DBG("end app");
}

Window* Application::createWindow() {
    float scalingFactor = 2;

    WindowProp prop;
    prop.width  = 160 * scalingFactor;
    prop.height = 144 * scalingFactor;

    Window* window = _windowManager->createWindow(prop);
    return window;
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
