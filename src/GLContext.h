#pragma once

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <Logging.h>
#include <Assert.h>

class GLContext {
   public:
    static void Init() {
        LOG_DBG("initializing context");
        glfwSetErrorCallback([](int error, const char* description) {
          LOG_ERROR("Code " << error << ": " << description);
        });
        int success = glfwInit();
        ASSERT(success == GLFW_TRUE, "Failed to initialize glfw");
    }

    static void Load() {
        LOG_DBG("loading GL");
        gladLoadGL(glfwGetProcAddress);
        glfwSwapInterval(1);
    }

    static void Destroy() {
        LOG_DBG("destroying context");
        glfwTerminate();
    }
};