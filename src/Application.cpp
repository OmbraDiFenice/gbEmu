#include <Pch.h>

#include "Application.h"

#include <core/emu/screen/TilePatternAdapter.h>
#include <core/emu/screen/Video.h>
#include <core/emu/screen/GbRenderer.h>

void Application::run() {
    LOG_DBG("start app");

    WindowProp prop;
    prop.width = prop.height = 640;

    Window* window = _windowManager->createWindow(prop);
    _components.push_back(window);

    window->setEventCallback(BIND_FN(Application::onEvent));

    TilePatternAdapter tileMapPatternAdapter;
    Video video(tileMapPatternAdapter);
    video.initialize();
    video.update();

    GbRenderer renderer = GbRenderer();

    while (keepRunning()) {
        renderer.render(video);

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

void Application::onKeyEvent(KeyEvent& e) { TRACE_CALL }

void Application::onMouseEvent(MouseEvent& e) { TRACE_CALL }

bool Application::keepRunning() { return WindowManager::GetWindowCount() > 0; }
