#ifndef GUI_BEHAVIOR_BEHAVIOR_H__INCLUDE
#define GUI_BEHAVIOR_BEHAVIOR_H__INCLUDE

#include "BehaviorDefs.h"

namespace FCGUI{
    //forward declaration
    class CWindow;

    class CBehaviorBase
    {
    public:
        CBehaviorBase(CWindow *window);
        virtual ~CBehaviorBase();

        virtual EBehaviorType Type() const = 0;

    protected:
        CWindow *m_pWindow;
    };
}

#endif