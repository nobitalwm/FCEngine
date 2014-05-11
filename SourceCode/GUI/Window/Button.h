#ifndef GUI_WINDOW_BUTTON_H__INCLUDE
#define GUI_WINDOW_BUTTON_H__INCLUDE

#include "Window.h"

namespace FCGUI
{
    class CButton : public CWindow
    {
        DECLARE_REFLECT_GUID(CButton, 0x6A2A717B, CWindow)
        DEFINE_WINDOW_TYPE(eWT_WINDOW_BUTTON);
    public:
        enum EEventType
        {
            eET_EVENT_BUTTON = FCGUI::eGUIET_EVENT_BUTTON,
            eET_EVENT_CLICKED,
        };

        enum EStateType
        {
            eST_STATE_NORMAL,
            eST_STATE_PRESSED,
            eST_STATE_DISABLED,

            eST_STATE_COUNT,
        };

    public:
        CButton();
        CButton(const TString &name);

        virtual void ReflectData(CSerializer& serializer) override;

        virtual int CurrState() const override;

        virtual bool OnMouseEvent( CMouseEvent *event ) override;

    protected:
        EStateType m_state;
    private:
        TString m_strNormalName;
        TString m_strPressedName;
        TString m_strDisabledName;
    };
}

#endif