#pragma once

#include <core/ui/Window.h>

class GLFWwindow;

class GLWindow : public Window {
   public:
    explicit GLWindow(GLFWwindow* glfWindow);
    GLWindow(GLWindow&& other) = default;

    void update() override;

    inline GLFWwindow* get() { return _glfWindow; }

   private:
    GLFWwindow* _glfWindow;
};

class GLWindowManager : public WindowManager {
   public:
    GLWindowManager();
    ~GLWindowManager() override;

    Window* createWindow(const WindowProp& prop) override;
    Window* createWindow() override;

   private:
    void setGlContext(GLFWwindow* window) const;
    static void DestroyWindow(GLFWwindow* window);
};