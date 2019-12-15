#pragma once
#include "Events/Event.h"


/**
 * Base class for all key events
 */
class KeyEvent : public Event
{
public:
    inline int getKeyCode() const noexcept { return m_keyCode;  }
    
protected:
    KeyEvent(int keyCode)
        : m_keyCode(keyCode) {}

    int m_keyCode;
};


class KeyPressEvent : public KeyEvent
{
public:
    EVENT_GET_INFO_IMPL(KeyPress)

    KeyPressEvent(int keyCode, bool isRepeat)
        : KeyEvent(keyCode), m_isRepeat(isRepeat) {}

    inline bool getRepeatState() const noexcept { return m_isRepeat; }

private:
    bool m_isRepeat;
};


class KeyReleaseEvent : public KeyEvent
{
public:
    EVENT_GET_INFO_IMPL(KeyRelease)

    KeyReleaseEvent(int keyCode)
        : KeyEvent(keyCode) {}
};