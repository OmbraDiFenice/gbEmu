#pragma once

#include <vector>
#include <Window.h>

class GLFWwindow;

class GLWindow : public Window {
   public:
    explicit GLWindow(GLFWwindow* glfWindow);
    GLWindow(GLWindow&& other) = default;

    void update() override;

   private:
    GLFWwindow* _glfWindow;
};

class GLWindowManager {
   public:
    static void Init();
    static void Destroy();
    static size_t GetWindowCount();

    static GLWindow CreateWindow(const WindowProp& prop);
    static GLWindow CreateWindow();

   private:
    static void SetGlContext(GLFWwindow* window);
    static void DestroyWindow(GLFWwindow* window);

   private:
    static std::vector<GLFWwindow*> windows;
};