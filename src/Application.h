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
    inline bool keepRunning();

   private:
    void onEvent(Event& e);
    void onKeyEvent(KeyEvent& e);
    void onMouseEvent(MouseEvent& e);

   private:
    std::vector<Component*> _components;
    std::unique_ptr<GLWindowManager> _windowManager;
};
