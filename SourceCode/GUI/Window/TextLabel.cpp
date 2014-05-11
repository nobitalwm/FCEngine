#include "stdafx.h"
#include "TextLabel.h"
#include "GUI/Event/WindowEvent.h"
#include "Event/MouseEvent.h"
#include "Event/KeyboardEvent.h"
#include "GUI/WindowManager.h"
#include "GUI/Font/FontManager.h"

using namespace FCGUI;

CTextLabel::CTextLabel()
{
    Init();
}

CTextLabel::CTextLabel( const TString &name )
    : CWindow(name)
{
    Init();
}

CTextLabel::~CTextLabel()
{

}

void CTextLabel::ReflectData(CSerializer& serializer)
{
    CWindow::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_fPadding, true, 0xFFFFFFFF, _T("±ß¿ò´óÐ¡"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strBackGroundName, true, 0xFFFFFFFF, _T("±³¾°Í¼"), NULL, NULL, NULL);
}

void CTextLabel::Init()
{
    m_fPadding = 3;
    m_strDisplayText = _T(""); 
    m_pFontFace  = CFontManager::GetInstance()->GetFace(_T("STFANGSO_12"));
}

bool CTextLabel::OnMouseEvent( CMouseEvent *event ) 
{
    bool bRet = false;
    if( IsEnabled() )
    {
        if(event->Button() == GLFW_MOUSE_BUTTON_LEFT)
        {
            if(event->Type() == eEVT_EVENT_MOUSE_PRESSED)
            {
                WindowEvent event(EVENT_TEXTLABEL_PRESSED);
                DispatchEvent(&event);
            }
        }
        bRet = true;
    }
    return bRet;
}
const TString& CTextLabel::GetDisplayText() const
{
    return m_strDisplayText;
}

void CTextLabel::SetText(const TString &text)
{
    m_strDisplayText = text;
}

kmScalar CTextLabel::GetPadding() const
{
    return m_fPadding;
}

CFontFace* CTextLabel::GetFontFace() const
{
    return m_pFontFace;
}

