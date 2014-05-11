#include "stdafx.h"
#include "Button.h"
#include "GUI/Event/WindowEvent.h"
#include "Event/MouseEvent.h"

using namespace FCGUI;

CButton::CButton()
{
}

CButton::CButton( const TString &name )
    : CWindow(name)
    , m_state(eST_STATE_NORMAL)
{

}

void CButton::ReflectData(CSerializer& serializer)
{
    CWindow::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_strNormalName, true, 0xFFFFFFFF, _T("按钮图"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strPressedName, true, 0xFFFFFFFF, _T("按钮图按下"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strDisabledName, true, 0xFFFFFFFF, _T("按钮图禁用"), NULL, NULL, NULL);
}

int CButton::CurrState() const
{
    return IsEnabled() ? m_state : eST_STATE_DISABLED;
}

bool CButton::OnMouseEvent( CMouseEvent *event )
{
    bool result = false;
    if(IsEnabled())
    {
        if(CWindow::OnMouseEvent(event))
        {
            if(event->Button() == GLFW_MOUSE_BUTTON_LEFT)
            {
                if(event->Type() == eEVT_EVENT_MOUSE_PRESSED)
                {
                    m_state = eST_STATE_PRESSED;
                }
                else if(event->Type() == eEVT_EVENT_MOUSE_RELEASED)
                {
                    if(m_state == eST_STATE_PRESSED)
                    {
                        m_state = eST_STATE_NORMAL;

                        WindowEvent event(eET_EVENT_CLICKED);
                        DispatchEvent(&event);
                    }
                }
            }

            result = true;
        }
    }

    return result;
}
