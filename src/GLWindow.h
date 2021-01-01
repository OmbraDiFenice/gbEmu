#pragma once

#include <string>

class GLFWwindow;

struct WindowProp {
    std::string title;
    size_t width, height;
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
