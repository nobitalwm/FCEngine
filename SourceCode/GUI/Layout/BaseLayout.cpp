#include "stdafx.h"
#include "BaseLayout.h"

using namespace FCGUI;

CBaseLayout::CBaseLayout()
    : m_pWindow(nullptr)
    , m_bInvalidated(true)
{

}

CBaseLayout::~CBaseLayout()
{

}

void CBaseLayout::SetWindow(CWindow *window)
{
    m_pWindow = window;
}

bool CBaseLayout::Invalidated() const
{
    return m_bInvalidated;
}

void CBaseLayout::PerformLayout()
{
    m_bInvalidated = false;
}

void CBaseLayout::invalidate()
{
    m_bInvalidated = true;
}

