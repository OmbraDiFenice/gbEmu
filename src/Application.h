#pragma once

#include <core/Component.h>
#include <core/Event.h>
#include <core/graphics/Window.h>
#include <core/graphics/opengl/GLWindowManager.h>
#include <memory>

class Application {
   public:
    Application() : _windowManager(new GLWindowManager()){};
    virtual ~Application() = default;

    void run();

   protected:
    void onEvent(Event& e);
    void onKeyEvent(KeyEvent& e);
    void onMouseEvent(MouseEvent& e);

   private:
    virtual void init()       = 0;
    virtual void update()     = 0;
    virtual void drawScreen() = 0;

    Window* createWindow();
    inline bool keepRunning();

   private:
    std::vector<Component*> _components;
    std::unique_ptr<GLWindowManager> _windowManager;
};
