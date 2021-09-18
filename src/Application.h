#pragma once

#include <core/Component.h>
#include <core/Event.h>
#include <core/ui/Window.h>
#include <core/ui/opengl/GLWindowManager.h>
#include <memory>

class Application {
   public:
    Application() : _windowManager(new GLWindowManager()){};

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
