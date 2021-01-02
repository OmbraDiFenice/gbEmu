#include "GLWindowManager.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

GLWindow::GLWindow(GLFWwindow* glfWindow) : _glfWindow(glfWindow) {};

void GLWindow::update() { glfwPollEvents(); }

std::vector<GLFWwindow*> GLWindowManager::windows;

void GLWindowManager::Init() {
    LOG_DBG("initializing window manager");

    glfwSetErrorCallback([](int error, const char* description) {
        LOG_ERROR("Code " << error << ": " << description);
    });

    int success = glfwInit();
    ASSERT(success == GLFW_TRUE, "Failed to initialize glfw");
}

void GLWindowManager::SetGlContext(GLFWwindow* window) {
    LOG_DBG("setting GL context");
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
}

void GLWindowManager::Destroy() {
    LOG_DBG("destroying window manager");
    for (GLFWwindow* window : windows) {
        LOG_DBG("destroying window");
        glfwDestroyWindow(window);
    }
    windows.clear();
    glfwTerminate();
}

void GLWindowManager::DestroyWindow(GLFWwindow* window) {
    LOG_DBG("destroying window");
    glfwDestroyWindow(window);
    for(auto it = windows.begin(); it != windows.end(); ++it) {
        if(*it == window) {
            windows.erase(it);
            break;
        }
    }
}

GLWindow&& GLWindowManager::CreateWindow(const WindowProp& prop) {
    LOG_DBG("creating window");

    GLFWwindow* glfWindow = glfwCreateWindow(prop.width, prop.height, prop.title.c_str(), nullptr, nullptr);
    ASSERT(glfWindow != nullptr, "failed to create window!");

    windows.emplace_back(glfWindow);

    glfwSetWindowCloseCallback(glfWindow, GLWindowManager::DestroyWindow);

    GLWindowManager::SetGlContext(glfWindow);

    return std::move(GLWindow(glfWindow));
}

GLWindow&& GLWindowManager::CreateWindow() {
    WindowProp props;
    return CreateWindow(props);
}

size_t GLWindowManager::GetWindowCount() { return windows.size(); }
