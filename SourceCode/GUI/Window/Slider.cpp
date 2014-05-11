#include "stdafx.h"
#include "Slider.h"
#include "GUI/Event/WindowEvent.h"
#include "Event/MouseEvent.h"
#include "GUI/Renderer/SliderRenderer.h"
#include "GUI/WindowManager.h"
#include "Render/TextureFragManager.h"

using namespace FCGUI;

CSlider::CSlider()
{
    Init();
}

CSlider::CSlider(const TString &name)
    : CWindow(name)
{
    Init();
}

void CSlider::ReflectData(CSerializer& serializer)
{
    CWindow::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_slierType, true, 0xFFFFFFFF, _T("水平/垂直"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_thumbSize, true, 0xFFFFFFFF, _T("滑块尺寸"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_nMinRange, true, 0xFFFFFFFF, _T("起始点位置"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_nMaxRange, true, 0xFFFFFFFF, _T("终止点位置"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_nPos, true, 0xFFFFFFFF, _T("滑动进度"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strThumbNormal, true, 0xFFFFFFFF, _T("滑块图"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strThumbPressed, true, 0xFFFFFFFF, _T("滑块图按下"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strThumbDisabled, true, 0xFFFFFFFF, _T("滑块图禁用"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strChanNormal, true, 0xFFFFFFFF, _T("通道图"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strChanDisable, true, 0xFFFFFFFF, _T("通道图禁用"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strChanFillNormal, true, 0xFFFFFFFF, _T("通道填充图"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strChanFillDisabled, true, 0xFFFFFFFF, _T("通道填充图禁用"), NULL, NULL, NULL);
}

void CSlider::Init()
{
    m_nMinRange = 0;
    m_nMaxRange = 100;
    m_nPos = 0;
    m_bPressed = false;
    m_slierType = eST_SLIDER_TYPE_HORIZONTAL;
    kmVec2Fill(&m_thumbSize, 11.f, 22.f);
    TCHAR name[32];
    _stprintf_s(name, _T("%p_SubBall"), this);
    m_pSubThumb = CWindowManager::GetInstance()->Create<CButton>(name, this);
    UpdatePos();
}

CSlider::~CSlider()
{

}

bool CSlider::OnMouseEvent( CMouseEvent *event )
{
    if(event->Button() == GLFW_MOUSE_BUTTON_LEFT)
    {
        if(event->Type() == eEVT_EVENT_MOUSE_PRESSED)
        {
            m_bPressed = true;
        }
        else if(event->Type() == eEVT_EVENT_MOUSE_RELEASED)
        {
            if(m_bPressed)
            {
                kmVec2 size = Size();
                kmVec3 pos;
                kmVec3Fill( &pos, event->X(),  event->Y(), 0.f );
                CWindow::WorldToLocal( pos );
               
                if(IsContainPos(pos))
                {
                    if(m_slierType == eST_SLIDER_TYPE_HORIZONTAL)
                    {
                        float percent = pos.x / size.x;
                        m_nPos = (int)(m_nMinRange + (m_nMaxRange - m_nMinRange) * percent);
                    }
                    else if(m_slierType == CSlider::eST_SLIDER_TYPE_VERTICAL)
                    {
                        float percent = pos.y / size.y;
                        m_nPos = (int)(m_nMinRange + (m_nMaxRange - m_nMinRange) * percent);
                    }

                    UpdatePos();
                    WindowEvent event(eET_EVENT_MOVE);
                    DispatchEvent(&event);
                }
                m_bPressed = false;
            }

        }
        else if(event->Type() == eEVT_EVENT_MOUSE_MOVED)
        {
            if(m_bPressed)
            {
                kmVec2 size = Size();
                kmVec3 pos;
                kmVec3Fill( &pos, event->X(),  event->Y(), 0.f );
                CWindow::WorldToLocal( pos );
                if(IsContainPos(pos))
                {
                    if(m_slierType == eST_SLIDER_TYPE_HORIZONTAL)
                    {
                        float percent = pos.x / size.x;
                        m_nPos = (int)(m_nMinRange + (m_nMaxRange - m_nMinRange) * percent);
                    }
                    else if(m_slierType == CSlider::eST_SLIDER_TYPE_VERTICAL)
                    {
                        float percent = pos.y / size.y;
                        m_nPos = (int)(m_nMinRange + (m_nMaxRange - m_nMinRange) * percent);
                    }

                    UpdatePos();
                    WindowEvent event(eET_EVENT_MOVE);
                    DispatchEvent(&event);
                }
            }
        }
    }
    return true;
}


void CSlider::SetRange(int nMin, int nMax)
{
    m_nMinRange = nMin;
    m_nMaxRange = nMax;
}

void CSlider::SetMinRange(int nMin)
{
    m_nMinRange = nMin;
}

void CSlider::SetMaxRange(int nMax)
{
    m_nMaxRange = nMax;
}

void CSlider::GetRange(int& nMin, int nMax)
{
    nMin = m_nMinRange;
    nMax = m_nMaxRange;
}

void CSlider::SetPos(int nPos)
{
    m_nPos = nPos;
}

int  CSlider::GetPos()
{
    return m_nPos;
}

void CSlider::SetThumbSize(kmVec2 size)
{
    m_thumbSize = size;
}

kmVec2 CSlider::GetThumbSize() const
{
    return m_thumbSize;
}

CButton* CSlider::GetSubThumb()
{
    return m_pSubThumb;
}

bool CSlider::IsContainPos(const kmVec3 &pos)
{
     kmVec2 size = RealSize();
     return (pos.x >=0 && pos.x <= size.x 
             && pos.y >=0 && pos.y <= size.y );
}

CSlider::ESliderType CSlider::GetSliderType()
{
    return m_slierType;
}

void CSlider::SetSliderType(ESliderType type)
{
    m_slierType =  type;
}

void CSlider::UpdatePos()
{
    CWindow *subBall = GetSubThumb();

    kmScalar fPercent = (float)m_nPos / (float)(m_nMaxRange - m_nMinRange);
    if( eST_SLIDER_TYPE_HORIZONTAL == m_slierType)
        subBall->SetArea(0.f, 0.f, fPercent, 0.f, m_thumbSize.x, 0.f, fPercent, 1.0f);
    else if(eST_SLIDER_TYPE_VERTICAL == m_slierType)
    {
        subBall->SetArea(0.f, 0.f, 0.0f, fPercent, 0.f, m_thumbSize.y, 1.0f, fPercent);
    }
}



