#pragma once

#include <string>

class GLFWwindow;

struct WindowProp {
    std::string title{"Title"};
    size_t width = 640;
    size_t height = 480;
};

class GLWindow {
   public:
    explicit GLWindow(const WindowProp& prop);

    void update();

    static unsigned int GetWindowCount();

   private:
    static void Destroy(GLFWwindow* window);

   private:
    static unsigned int windowCount;
    GLFWwindow* glfWindow;
};
