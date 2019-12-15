#pragma once


/**
 * Unique integer codes for all events
 */
enum class EventCode
{
    None = 0,
    WindowClose, WindowResize,
    KeyPress, KeyRelease, KeyRepeat,
    MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll
};


/**
 * Interface for all events
 */
class Event
{
public:
    bool m_isHandled = false;
    virtual EventCode getType() const noexcept = 0;

#ifdef PS_DEBUG
    virtual const char* getName() const noexcept = 0;
#endif // PS_DEBUG
};


/**
 * Macro with definitions of common events functions
 */
#ifdef PS_DEBUG
    #define EVENT_GET_INFO_IMPL(x) \
        virtual EventCode getType() const noexcept override { return EventCode::##x; } \
        virtual const char* getName() const noexcept { return #x; }
#elif PS_RELEASE
    #define EVENT_GET_INFO_IMPL(x) \
        virtual EventCode getType() const noexcept override { return EventCode::##x; }
#endif // PS_DEBUG