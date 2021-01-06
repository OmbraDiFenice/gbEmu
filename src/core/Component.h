#pragma once

#include <core/Event.h>

class Component {
   public:
    virtual void update() = 0;
    virtual void onEvent(Event& e) {};
};