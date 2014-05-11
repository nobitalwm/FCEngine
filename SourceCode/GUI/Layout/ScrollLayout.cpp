#include "stdafx.h"
#include "ScrollLayout.h"
#include "GUI/Window/Window.h"

using namespace FCGUI;

CScrollLayout::CScrollLayout( size_t rowCount, size_t colCount )
    : CGridLayout(rowCount, colCount)
{

}

bool CScrollLayout::AddChild( CWindow *window )
{
    BEATS_ASSERT(m_pWindow, _T("You must set this layout to a window first"));
    BEATS_ASSERT(m_pWindow->GetChild(window->Name()), 
        _T("window:%s must be the child of window:%s"), window->Name().c_str(), m_pWindow->Name().c_str());

    for(size_t i = 0; i < m_children.size(); ++i)
    {
        if(!m_children[i])
        {
            m_children[i] = window;
            PerformLayout();
            return true;
        }
    }

    SetRowCount(m_uRowCount+1);
    m_children[m_children.size() - m_uColCount] = window;
    invalidate();
    return true;
}

bool CScrollLayout::AddChildToCell( CWindow *window, size_t row, size_t col )
{
    BEATS_ASSERT(m_pWindow, _T("You must set this layout to a window first"));
    BEATS_ASSERT(m_pWindow->GetChild(window->Name()), 
        _T("window:%s must be the child of window:%s"), window->Name().c_str(), m_pWindow->Name().c_str());

    size_t index = row * m_uColCount + col;
    if(index >= m_children.size())
    {
        return AddChild(window);
    }

    if(m_children[index])
    {
        if(m_children.back())
        {
            SetRowCount(m_uRowCount+1);
        }
        for(size_t i = m_children.size()-1; i > index; --i)
        {
            m_children[i] = m_children[i-1];
        }
    }
    m_children[index] = window;
    invalidate();
    return true;
}

bool CScrollLayout::RemoveChild( CWindow *window )
{
    auto itr = std::find(m_children.begin(), m_children.end(), window);
    if(itr != m_children.end())
    {
        for(auto itrNext = itr + 1; itrNext != m_children.end(); ++itrNext, ++itr)
        {
            *itr = *itrNext;
        }
        *itr = nullptr;

        if(!m_children[m_children.size()-m_uColCount])
            SetRowCount(m_uRowCount-1);
        invalidate();
        return true;
    }
    return false;
}
