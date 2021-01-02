#include "Application.h"

#include <GLWindowManager.h>

Application::Application() { GLWindowManager::Init(); }
Application::~Application() { GLWindowManager::Destroy(); }

void Application::run() {
    LOG_DBG("start app");

    GLWindow window = GLWindowManager::CreateWindow();
    _components.push_back(&window);

    while (GLWindowManager::GetWindowCount() > 0) {
        for(auto c : _components) {
            c->update();
        }
    }

    LOG_DBG("end app");
}
