#include "GLWindow.h"
#include <GLContext.h>
#include <GLFW/glfw3.h>

unsigned int GLWindow::windowCount = 0;

GLWindow::GLWindow(const WindowProp& prop) {
    LOG_DBG("creating window");

    glfWindow = glfwCreateWindow(prop.width, prop.height, prop.title.c_str(), nullptr, nullptr);
    ASSERT(glfWindow != nullptr, "failed to create window!");

    ++windowCount;

    glfwSetWindowCloseCallback(glfWindow, GLWindow::Destroy);
    glfwMakeContextCurrent(glfWindow);

    GLContext::Load();
};

void GLWindow::Destroy(GLFWwindow* window) {
    LOG_DBG("destroying window");
    glfwDestroyWindow(window);
    --windowCount;

    if (windowCount == 0) {
        GLContext::Destroy();
    }
};

void GLWindow::update() { glfwPollEvents(); }

unsigned int GLWindow::GetWindowCount() { return windowCount; }
