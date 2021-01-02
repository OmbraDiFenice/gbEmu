#pragma once

#include <string>
#include <Application.h>

struct WindowProp {
    std::string title{"Title"};
    size_t width = 640;
    size_t height = 480;
};

class Window : public Component {};