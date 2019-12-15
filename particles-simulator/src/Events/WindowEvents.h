#pragma once
#include "Events/Event.h"


class WindowCloseEvent : public Event
{
public:
    EVENT_GET_INFO_IMPL(WindowClose)
};


class WindowResizeEvent : public Event
{
public:
    EVENT_GET_INFO_IMPL(WindowResize)

    WindowResizeEvent(unsigned int width, unsigned int height)
        : m_width(width), m_height(height)
    { }

    inline unsigned int getWidth() const noexcept { return m_width; }
    inline unsigned int getHeight() const noexcept { return m_height; }

private:
    unsigned int m_width, m_height;
};