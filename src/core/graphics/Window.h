#pragma once

#include <core/Component.h>
#include <core/Event.h>
#include <functional>
#include <string>
#include <utility>
#include <vector>

struct WindowProp {
    std::string title{"Title"};
    size_t width = 640;
    size_t height = 480;
};

class Window : public Component {
   public:
    /** Register the main event callback.
     *
     * This callback will capture all the events generated by the window in one
     * single function. The events are supposed to then be dispatched to each
     * component.
     *
     * The callback registered here is supposed to be handled at application
     * level.
     */
    virtual void setEventCallback(std::function<void(Event&)> cb) {
        _eventCallback = std::move(cb);
    }

   protected:
    std::function<void(Event&)> _eventCallback;
};

class WindowManager {
   public:
    WindowManager() = default;
    virtual ~WindowManager() = default;

    virtual Window* createWindow(const WindowProp& prop) = 0;
    virtual Window* createWindow() = 0;
    static inline size_t GetWindowCount() { return windows.size(); }

   protected:
    static std::vector<Window*> windows;
};