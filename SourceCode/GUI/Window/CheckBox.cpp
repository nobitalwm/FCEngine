#include "stdafx.h"
#include "CheckBox.h"
#include "GUI/Event/WindowEvent.h"
#include "Event/MouseEvent.h"
#include "GUI/WindowManager.h"
#include "Render/TextureFragManager.h"
#include "GUI/Renderer/CheckBoxRenderer.h"

using namespace FCGUI;

CCheckBox::CCheckBox()
{
    Init();
}

CCheckBox::CCheckBox(const TString &name)
    : CWindow(name)
{
    Init();
}

CCheckBox::~CCheckBox()
{

}

void CCheckBox::ReflectData(CSerializer& serializer)
{
    CWindow::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_bCheck, true, 0xFFFFFFFF, _T("选中/取消"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_fContentWidth, true, 0xFFFFFFFF, _T("标签宽度"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strIndicatorNormalCheck, true, 0xFFFFFFFF, _T("标识图选中"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strIndicatorNormalUnCheck, true, 0xFFFFFFFF, _T("标识图未选中"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strIndicatorDisabledCheck, true, 0xFFFFFFFF, _T("标识图选中禁用"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strIndicatorDisabledUnCheck, true, 0xFFFFFFFF, _T("标识图未选中禁用"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strContentNormal, true, 0xFFFFFFFF, _T("内容描述图"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strContentDisabled, true, 0xFFFFFFFF, _T("内容描述图禁用"), NULL, NULL, NULL);
}

void CCheckBox::InitChild()
{
     TCHAR name[32];
     _stprintf_s(name, _T("%p_SubContent"), this);
    m_pSubContent = CWindowManager::GetInstance()->Create<CWindow>(name, this);
    m_pSubContent->SetArea( 0.f, 0.f, 1.f, 0.f, m_fContentWidth, 0.f, 1.f, 1.f );
}

void CCheckBox::Init()
{
    m_bCheck = false;
    m_state = eST_STATE_NORMAL_UNCHECKED;
    m_fContentWidth = 120.f ;

    InitChild();
}

int CCheckBox::CurrState() const
{
    EStateType state = m_state;
    if(!IsEnabled())
    {
        if(m_bCheck)
            state = eST_STATE_DISABLED_CHECKED;
        else
            state = eST_STATE_DISABLED_UNCHECKED;
    }
    return state;
}

CWindow* CCheckBox::GetSubWindow()
{
        return m_pSubContent;
}

bool CCheckBox::OnMouseEvent( CMouseEvent *event )
{
    bool bRet = false;
    if(IsEnabled() && CWindow::OnMouseEvent(event))
    {
        if(event->Button() == GLFW_MOUSE_BUTTON_LEFT)
        {
            if(event->Type() == eEVT_EVENT_MOUSE_RELEASED)
            { 
                m_bCheck = !m_bCheck;
                if(m_bCheck)
                {
                    m_state = eST_STATE_NORMAL_CHECKED;
                }
                else
                {
                    m_state = eST_STATE_NORMAL_UNCHECKED;
                }
               
                WindowEvent event(eET_EVENT_CHECK);
                DispatchEvent(&event);
            }
        }
        bRet = true;
    }
    return bRet;
}

bool CCheckBox::IsCheck() const
{
    return m_bCheck;
}

void CCheckBox::SetCheck(bool bCheck)
{
    if(IsEnabled() && m_bCheck != bCheck)
    {
        m_bCheck = bCheck;

        if(m_bCheck)
        {
            m_state = eST_STATE_NORMAL_CHECKED;
        }
        else
        {
            m_state = eST_STATE_NORMAL_UNCHECKED;
        }

        WindowEvent event(eET_EVENT_CHECK);
        DispatchEvent(&event);
    }
}

