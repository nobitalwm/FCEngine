#include "stdafx.h"
#include "Progress.h"
#include "GUI/Event/WindowEvent.h"
#include "Event/MouseEvent.h"
#include "GUI/WindowManager.h"

using namespace FCGUI;

CProgress::CProgress()
{
    Init();
}

CProgress::CProgress(const TString &name)
    :CWindow(name)
{
    Init();
}

CProgress::~CProgress()
{
}

void CProgress::ReflectData(CSerializer& serializer)
{
    CWindow::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_nRange, true, 0xFFFFFFFF, _T("进度范围"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_nPos, true, 0xFFFFFFFF, _T("进度"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_nStep, true, 0xFFFFFFFF, _T("步长"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strBackGroundName, true, 0xFFFFFFFF, _T("背景图"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strFillName, true, 0xFFFFFFFF, _T("填充图"), NULL, NULL, NULL);
}

void CProgress::Init()
{
    m_nRange = 100;
    m_nPos = 0;
    m_nStep = 1;
    TCHAR name[32];
    _stprintf_s(name, _T("%p_SubFill"), this);
    m_pSubWndFill = CWindowManager::GetInstance()->Create<CWindow>(name, this);
    UpdateProgress();
}

void CProgress::SetRange(int nRange)
{
    m_nRange = nRange;
}

int CProgress::GetRange() const
{
    return m_nRange;
}

void CProgress::SetPos(int nPos)
{
    m_nPos = nPos;

    UpdateProgress();
}

int CProgress::GetPos()
{
    return m_nPos;
}

int CProgress::SetStep(int nStep)
{
    int nOldStep = m_nStep;
    m_nStep = nStep;

    return nOldStep;
}

void CProgress::Step()
{
    m_nPos += m_nStep;

    UpdateProgress();
}

CWindow* CProgress::GetSubWndFill()
{
    return m_pSubWndFill;
}

void CProgress::UpdateProgress()
{
    float fPercent = (float)(m_nPos) / (float)m_nRange;
    GetSubWndFill()->SetArea(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, fPercent, 1.f);
}


