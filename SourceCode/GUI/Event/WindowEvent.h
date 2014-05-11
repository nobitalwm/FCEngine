#ifndef GUI_EVENT_WINDOW_EVENT_H__INCLUDE
#define GUI_EVENT_WINDOW_EVENT_H__INCLUDE

#include "Event/BaseEvent.h"

namespace FCGUI
{
    class CWindow;

    class WindowEvent : public CBaseEvent
    {
    public:
        WindowEvent(int type, CWindow *operand = nullptr)
            : CBaseEvent(type)
            , _operand(operand)
        {}

        CWindow *SourceWindow() const
        {
            return static_cast<CWindow *>(m_pSource);
        }

        CWindow *OperandWindow() const
        {
            return _operand;
        }

    protected:
        CWindow *_operand;
    };
}

#endif