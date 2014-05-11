#include "stdafx.h"
#include "HScrollLayout.h"
#include "GUI/Window/Window.h"

using namespace FCGUI;

CHScrollLayout::CHScrollLayout( size_t colCount /*= 1*/ )
    : CScrollLayout(0, colCount)
{

}

bool CHScrollLayout::AddChild( CWindow *window, kmScalar x, kmScalar y )
{
    BEATS_ASSERT(m_pWindow, _T("You must set this layout to a window first"));
    BEATS_ASSERT(m_pWindow->GetChild(window->Name()), 
        _T("window:%s must be the child of window:%s"), window->Name().c_str(), m_pWindow->Name().c_str());

    if(Invalidated())
        PerformLayout();
    
    kmVec2 realAnchor = m_pWindow->RealAnchor();
    x += realAnchor.x;
    y += realAnchor.y;

    int row = -1;
    for(size_t i = 0; i < m_uRowCount; ++i)
    {
        kmScalar left = m_vSplitPos[2*i].second * m_pWindow->RealSize().x + m_vSplitPos[2*i].first;
        kmScalar right = m_vSplitPos[2*i+1].second * m_pWindow->RealSize().x + m_vSplitPos[2*i+1].first;
        if(left <= x && x <= right)
        {
            row = i;
            break;
        }
    }

    int col = -1;
    for(size_t i = 0; i < m_uColCount; ++i)
    {
        kmScalar top = m_hSplitPos[2*i].second * m_pWindow->RealSize().y + m_hSplitPos[2*i].first;
        kmScalar bottom = m_hSplitPos[2*i+1].second * m_pWindow->RealSize().y + m_hSplitPos[2*i+1].first;
        if(top <= y && y <= bottom)
        {
            col = i;
            break;
        }
    }

    return row >= 0 && col >= 0 ? AddChildToCell(window, row, col) : CScrollLayout::AddChild(window);
}

void CHScrollLayout::PerformLayout()
{
    CalcSplitPos();

    //set position and size of child window
    for(size_t row = 0; row < m_uRowCount; ++row)
    {
        std::pair<kmScalar, kmScalar> left = m_vSplitPos[2*row];
        std::pair<kmScalar, kmScalar> right = m_vSplitPos[2*row+1];
        for(size_t col = 0; col < m_uColCount; ++col)
        {
            size_t index = row * m_uColCount + col;
            CWindow *child = m_children[index];
            if(child)
            {
                std::pair<kmScalar, kmScalar> top = m_hSplitPos[2*col];
                std::pair<kmScalar, kmScalar> bottom = m_hSplitPos[2*col+1];
                child->SetArea(left.first, top.first, left.second, top.second,
                    right.first, bottom.first, right.second, bottom.second);
            }
        }
    }

    CBaseLayout::PerformLayout();
}

