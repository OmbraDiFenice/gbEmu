#pragma once

#include <core/Component.h>
#include <core/Event.h>

class Application {
   public:
    Application();
    ~Application();

    void run();

   private:
    void onEvent(Event& e);
    void onKeyEvent(KeyEvent& e);
    void onMouseEvent(MouseEvent& e);

   private:
    std::vector<Component*> _components;
};
