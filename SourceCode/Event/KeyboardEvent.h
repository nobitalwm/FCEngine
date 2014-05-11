#ifndef EVENT_KEYBOARD_EVENT_H__INCLUDE
#define EVENT_KEYBOARD_EVENT_H__INCLUDE

#include "Event/BaseEvent.h"

class CKeyboardEvent : public CBaseEvent
{
public:
    CKeyboardEvent(int type, int key, int mods = 0)
        : CBaseEvent(type)
        , m_nKey(key)
        , m_nMods(mods)
    {}

    int Key() const
    {
        return m_nKey;
    }

    bool IsControlKeyDown(int ctrlKey) const
    {
        return (m_nMods & ctrlKey) != 0;
    }

    char Char() const
    {
        return static_cast<char>(m_nKey);
    }

    wchar_t WChar() const
    {
        return static_cast<wchar_t>(m_nKey);
    }

private:
    int m_nKey;
    int m_nMods;
};

#endif