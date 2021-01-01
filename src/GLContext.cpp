#include "GLContext.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

void GLContext::Init() {
    LOG_DBG("initializing context");
    glfwSetErrorCallback([](int error, const char* description) {
      LOG_ERROR("Code " << error << ": " << description);
    });
    int success = glfwInit();
    ASSERT(success == GLFW_TRUE, "Failed to initialize glfw");
}

void GLContext::Load() {
    LOG_DBG("loading GL");
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
}

void GLContext::Destroy() {
    LOG_DBG("destroying context");
    glfwTerminate();
}
