#include <Pch.h>

#include "GLWindowManager.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <utils/GLErrorMacros.h>

GLWindow::GLWindow(GLFWwindow* glfWindow) : _glfWindow(glfWindow) {
    glfwSetWindowUserPointer(glfWindow, this);
    glfwSetKeyCallback(glfWindow, [](GLFWwindow* window, int code, int key,
                                     int action, int mods) {
        auto* self = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
        KeyEvent e;
        self->_eventCallback(e);
    });
    glfwSetMouseButtonCallback(glfWindow, [](GLFWwindow* window, int button,
                                             int action, int mods) {
        auto* self = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
        MouseEvent e;
        self->_eventCallback(e);
    });
}

void GLWindow::update() {
    glfwSwapBuffers(_glfWindow);
    glfwPollEvents();
}

GLWindowManager::GLWindowManager() {
    TRACE_CALL

    glfwSetErrorCallback([](int error, const char* description) {
        LOG_ERROR("Code " << error << ": " << description);
    });

    int success = glfwInit();
    ASSERT(success == GLFW_TRUE, "Failed to initialize glfw");
}

GLWindowManager::~GLWindowManager() {
    TRACE_CALL
    for (Window* window : windows) {
        glfwDestroyWindow(static_cast<GLWindow*>(window)->get());
    }
    GLWindowManager::windows.clear();
    glfwTerminate();
}

Window* GLWindowManager::createWindow(const WindowProp& prop) {
    TRACE_CALL

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* glfWindow = glfwCreateWindow(
        prop.width, prop.height, prop.title.c_str(), nullptr, nullptr);
    ASSERT(glfWindow != nullptr, "failed to create window!");

    auto* window = new GLWindow{glfWindow};

    windows.emplace_back(window);

    glfwSetWindowCloseCallback(glfWindow, GLWindowManager::DestroyWindow);

    setGlContext(glfWindow);

    LOG_INFO("Vendor: " << glGetString(GL_VENDOR));
    LOG_INFO("Renderer: " << glGetString(GL_RENDERER));
    LOG_INFO("Version: " << glGetString(GL_VERSION));

    return window;
}

Window* GLWindowManager::createWindow() {
    WindowProp props;
    return createWindow(props);
}

void GLWindowManager::setGlContext(GLFWwindow* window) const {
    TRACE_CALL
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));
}

void GLWindowManager::DestroyWindow(GLFWwindow* window) {
    TRACE_CALL
    glfwDestroyWindow(window);
    for (auto it = windows.begin(); it != windows.end(); ++it) {
        auto* storedWindow = static_cast<GLWindow*>(*it);
        if (storedWindow->get() == window) {
            windows.erase(it);
            break;
        }
    }
}
