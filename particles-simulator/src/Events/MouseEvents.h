#pragma once
#include "Events/Event.h"


class MouseMoveEvent : public Event
{
public:
    EVENT_GET_INFO_IMPL(MouseMove)

    MouseMoveEvent(double x, double y)
        : m_posX(x), m_posY(y) {}

    inline double getX() const noexcept { return m_posX; }
    inline double getY() const noexcept { return m_posY; }

private:
    double m_posX;
    double m_posY;
};


/**
 * Base class for mouse click events
 */
class MouseButtonEvent : public Event
{
public:
    inline int getButton() const noexcept { return m_button; }

protected:
    MouseButtonEvent(int button)
        : m_button(button) {}

    int m_button;
};


class MouseButtonPressEvent : public MouseButtonEvent
{
public:
    EVENT_GET_INFO_IMPL(MouseButtonPress)

    MouseButtonPressEvent(int button)
        : MouseButtonEvent(button) {}
};


class MouseButtonReleaseEvent : public MouseButtonEvent
{
public:
    EVENT_GET_INFO_IMPL(MouseButtonRelease)

    MouseButtonReleaseEvent(int button)
        : MouseButtonEvent(button) {}
};