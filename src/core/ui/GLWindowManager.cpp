#include "GLWindowManager.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

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
};

void GLWindow::update() {
    glfwPollEvents();

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

    glfwSwapBuffers(_glfWindow);
}

std::vector<GLFWwindow*> GLWindowManager::windows;

void GLWindowManager::Init() {
    TRACE_CALL

    glfwSetErrorCallback([](int error, const char* description) {
        LOG_ERROR("Code " << error << ": " << description);
    });

    int success = glfwInit();
    ASSERT(success == GLFW_TRUE, "Failed to initialize glfw");
}

void GLWindowManager::SetGlContext(GLFWwindow* window) {
    TRACE_CALL
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
}

void GLWindowManager::Destroy() {
    TRACE_CALL
    for (GLFWwindow* window : windows) {
        glfwDestroyWindow(window);
    }
    windows.clear();
    glfwTerminate();
}

void GLWindowManager::DestroyWindow(GLFWwindow* window) {
    TRACE_CALL
    glfwDestroyWindow(window);
    for (auto it = windows.begin(); it != windows.end(); ++it) {
        if (*it == window) {
            windows.erase(it);
            break;
        }
    }
}

GLWindow GLWindowManager::CreateWindow(const WindowProp& prop) {
    TRACE_CALL

    GLFWwindow* glfWindow = glfwCreateWindow(
        prop.width, prop.height, prop.title.c_str(), nullptr, nullptr);
    ASSERT(glfWindow != nullptr, "failed to create window!");

    windows.emplace_back(glfWindow);

    glfwSetWindowCloseCallback(glfWindow, GLWindowManager::DestroyWindow);

    GLWindowManager::SetGlContext(glfWindow);

    return GLWindow(glfWindow);
}

GLWindow GLWindowManager::CreateWindow() {
    WindowProp props;
    return CreateWindow(props);
}

size_t GLWindowManager::GetWindowCount() { return windows.size(); }
