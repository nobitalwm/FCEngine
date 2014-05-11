#ifndef EVENT_MOUSE_EVENT_H__INCLUDE
#define EVENT_MOUSE_EVENT_H__INCLUDE

#include "Event/BaseEvent.h"

class CMouseEvent : public CBaseEvent
{
public:
    CMouseEvent(int type, int button, char buttonState, kmScalar x = 0.f, kmScalar y = 0.f)
        : CBaseEvent(type)
        , m_nButton(button)
        , m_nButtonState(buttonState)
        , m_fX(x)
        , m_fY(y)
    {
    }

    int Button() const
    {
        return m_nButton;
    }

    bool IsButtonDown(int button) const
    {
        return ((m_nButtonState >> button) & 1) != 0;
    }

    kmScalar X() const
    {
        return m_fX;
    }

    kmScalar Y() const
    {
        return m_fY;
    }

private:
    int m_nButton;
    char m_nButtonState;
    kmScalar m_fX;
    kmScalar m_fY;
};

#endif