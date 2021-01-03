#pragma once

#include <vector>
#include <Event.h>

class Component {
   public:
    virtual void update() = 0;
    virtual void onEvent(Event& e) {};
};

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
