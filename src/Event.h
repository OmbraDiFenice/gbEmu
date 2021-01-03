#pragma once

#define EVENT_DEF(x)                                     \
    std::string getType() override { return GetType(); } \
    static std::string GetType() { return #x; }

struct Event {
    virtual std::string getType() = 0;
};

class EventDispatcher {
   public:
    template <typename E, typename F>
    void dispatch(F handler, Event& e) {
        if (e.getType() == E::GetType()) {
            handler(static_cast<E&>(e));
        }
    }
};

struct KeyEvent : public Event {
    EVENT_DEF(KeyEvent);
};

struct MouseEvent : public Event {
    EVENT_DEF(MouseEvent);
};