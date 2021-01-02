#pragma once

#include <vector>

class Component {
   public:
    virtual void update() = 0;
};

class Application {
   public:
    Application();
    ~Application();

    void run();

   private:
    std::vector<Component*> _components;
};
