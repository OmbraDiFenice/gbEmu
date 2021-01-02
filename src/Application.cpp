#include "Application.h"

#include <GLWindowManager.h>

Application::Application() { GLWindowManager::Init(); }
Application::~Application() { GLWindowManager::Destroy(); }

void Application::run() {
    LOG_DBG("start app");

    GLWindow window = GLWindowManager::CreateWindow();

    while (GLWindowManager::GetWindowCount() > 0) {
        window.update();
    }

    LOG_DBG("end app");
}
