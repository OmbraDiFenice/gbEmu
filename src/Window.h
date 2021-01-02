#pragma once

#include <string>

struct WindowProp {
    std::string title{"Title"};
    size_t width = 640;
    size_t height = 480;
};

class Window {
   public:
    virtual void update() = 0;
};