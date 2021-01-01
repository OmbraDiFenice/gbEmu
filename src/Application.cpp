#include "Application.h"

#include <GLContext.h>
#include <Logging.h>

Application::Application() {
    GLContext::Init();
}

void Application::run() {
    LOG_DBG("start app");

    WindowProp props;
    GLWindow window(props);

    while (GLWindow::GetWindowCount() > 0) {
        window.update();
    }

    LOG_DBG("end app");
}
