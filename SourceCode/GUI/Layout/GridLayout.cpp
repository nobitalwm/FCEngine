#include "stdafx.h"
#include "GridLayout.h"
#include "GUI/Window/Window.h"
#include "GUI/Event/WindowEvent.h"

using namespace FCGUI;

CGridLayout::CGridLayout( size_t rowCount, size_t colCount )
    : m_uRowCount(rowCount)
    , m_uColCount(colCount)
    , m_fTopMargin(0.f)
    , m_fBottomMargin(0.f)
    , m_fLeftMargin(0.f)
    , m_fRightMargin(0.f)
    , m_fHGap(0.f)
    , m_fVGap(0.f)
    , m_fColWidth(0.f)
    , m_fRowHeight(0.f)
{
    m_children.resize(rowCount*colCount);
    m_everyRowHeight.resize(rowCount);
    m_everyColWidth.resize(colCount);
}

CGridLayout::~CGridLayout()
{

}

void CGridLayout::SetWindow(CWindow *window)
{
    CBaseLayout::SetWindow(window);
    window->SubscribeEvent(CWindow::EVENT_CHILD_REMOVE, [this](CBaseEvent *event) {
        WindowEvent *wndEvt = static_cast<WindowEvent *>(event);
        this->RemoveChild(wndEvt->OperandWindow());
    });
}

void CGridLayout::SetRowCount( size_t rowCount )
{
    m_children.resize(rowCount*m_uColCount);
    m_everyRowHeight.resize(rowCount);
    m_uRowCount = rowCount;
    invalidate();
}

size_t CGridLayout::RowCount() const
{
    return m_uRowCount;
}

void CGridLayout::SetColCount( size_t colCount )
{
    if(colCount > m_uColCount)
    {
        m_children.resize(m_uRowCount * colCount);
        //column count changed, move the existed item to corresponding position
        for(int i = m_uRowCount*m_uColCount-1; i >= 0; --i)
        {
            if(m_children[i])
            {
                size_t row = i / m_uColCount;
                size_t col = i % m_uColCount;
                int newidx = row*colCount + col;
                if(newidx != i)
                {
                    m_children[newidx] = m_children[i];
                    m_children[i] = nullptr;
                }
            }
        }
        m_uColCount = colCount;
        m_everyColWidth.resize(colCount);
        invalidate();
    }
    else if(colCount < m_uColCount)
    {
        for(size_t i = 0; i < m_children.size(); ++i)
        {
            if(m_children[i])
            {
                size_t row = i / m_uColCount;
                size_t col = i % m_uColCount;
                size_t newidx = row*colCount + col;
                if(newidx != i)
                {
                    m_children[newidx] = m_children[i];
                    m_children[i] = nullptr;
                }
            }
        }
        m_children.resize(m_uRowCount * colCount);
        m_everyColWidth.resize(colCount);
        m_uColCount = colCount;
        invalidate();
    }
}

size_t CGridLayout::ColCount() const
{
    return m_uColCount;
}

void CGridLayout::SetMargin( kmScalar topMargin, kmScalar bottomMargin, 
    kmScalar leftMargin, kmScalar rightMargin )
{
    m_fTopMargin = topMargin;
    m_fBottomMargin = bottomMargin;
    m_fLeftMargin = leftMargin;
    m_fRightMargin = rightMargin;
    invalidate();
}

kmScalar CGridLayout::TopMargin() const
{
    return m_fTopMargin;
}

kmScalar CGridLayout::BottomMargin() const
{
    return m_fBottomMargin;
}

kmScalar CGridLayout::LeftMargin() const
{
    return m_fLeftMargin;
}

kmScalar CGridLayout::RightMargin() const
{
    return m_fRightMargin;
}

void CGridLayout::SetGap( kmScalar hGap, kmScalar vGap )
{
    m_fHGap = hGap;
    m_fVGap = vGap;
    invalidate();
}

kmScalar CGridLayout::HGap() const
{
    return m_fHGap;
}

kmScalar CGridLayout::VGap() const
{
    return m_fVGap;
}

void CGridLayout::SetRowHeight( kmScalar rowHeight )
{
    m_fRowHeight = rowHeight;
    invalidate();
}

void CGridLayout::SetColWidth( kmScalar colWidth )
{
    m_fColWidth = colWidth;
    invalidate();
}

void CGridLayout::SetRowHeightSingleFixed( size_t row, kmScalar height)
{
    BEATS_ASSERT(row < m_uRowCount);
    m_everyRowHeight.resize(m_uRowCount);
    m_everyRowHeight[row] = std::make_pair(height, 0.f);
    invalidate();
}

void CGridLayout::SetRowHeightSingleFactored( size_t row, kmScalar heightFactor)
{
    BEATS_ASSERT(row < m_uRowCount);
    m_everyRowHeight.resize(m_uRowCount);
    m_everyRowHeight[row] = std::make_pair(0.f, heightFactor);
    invalidate();
}

void CGridLayout::SetColWidthSingleFixed( size_t col, kmScalar width)
{
    BEATS_ASSERT(col < m_uColCount);
    m_everyColWidth.resize(m_uColCount);
    m_everyColWidth[col] = std::make_pair(width, 0.f);
    invalidate();
}

void CGridLayout::SetColWidthSingleFactored( size_t col, kmScalar widthFactor)
{
    BEATS_ASSERT(col < m_uColCount);
    m_everyColWidth.resize(m_uColCount);
    m_everyColWidth[col] = std::make_pair(0.f, widthFactor);
    invalidate();
}

bool CGridLayout::AddChild(CWindow *window)
{
    BEATS_ASSERT(m_pWindow, _T("You must set this layout to a window first"));
    BEATS_ASSERT(m_pWindow->GetChild(window->Name()), 
        _T("window:%s must be the child of window:%s"), window->Name().c_str(), m_pWindow->Name().c_str());

    for(size_t i = 0; i < m_children.size(); ++i)
    {
        if(!m_children[i])
        {
            m_children[i] = window;
            invalidate();
            return true;
        }
    }

    return false;
}

bool CGridLayout::AddChild( CWindow *window, kmScalar x, kmScalar y )
{
    BEATS_ASSERT(m_pWindow, _T("You must set this layout to a window first"));
    BEATS_ASSERT(m_pWindow->GetChild(window->Name()), 
        _T("window:%s must be the child of window:%s"), window->Name().c_str(), m_pWindow->Name().c_str());

    if(Invalidated())
        PerformLayout();
    
    kmVec2 realAnchor = m_pWindow->RealAnchor();
    x += realAnchor.x;
    y += realAnchor.y;

    int col = -1;
    for(size_t i = 0; i < m_uColCount; ++i)
    {
        kmScalar left = m_hSplitPos[2*i].second * m_pWindow->RealSize().x + m_hSplitPos[2*i].first;
        kmScalar right = m_hSplitPos[2*i+1].second * m_pWindow->RealSize().x + m_hSplitPos[2*i+1].first;
        if(left <= x && x <= right)
        {
            col = i;
            break;
        }
    }

    int row = -1;
    for(size_t i = 0; i < m_uRowCount; ++i)
    {
        kmScalar top = m_vSplitPos[2*i].second * m_pWindow->RealSize().y + m_vSplitPos[2*i].first;
        kmScalar bottom = m_vSplitPos[2*i+1].second * m_pWindow->RealSize().y + m_vSplitPos[2*i+1].first;
        if(top <= y && y <= bottom)
        {
            row = i;
            break;
        }
    }

    return row >= 0 && col >= 0 ? AddChildToCell(window, row, col) : AddChild(window);
}

bool CGridLayout::AddChildToCell( CWindow *window, size_t row, size_t col)
{
    BEATS_ASSERT(m_pWindow, _T("You must set this layout to a window first"));
    BEATS_ASSERT(m_pWindow->GetChild(window->Name()), 
        _T("window:%s must be the child of window:%s"), window->Name().c_str(), m_pWindow->Name().c_str());
   
    if(row < m_uRowCount && col < m_uColCount)
    {
        size_t index = row*m_uColCount+col;
        if(!m_children[index])    //already has item
        {
            m_children[index] = window;
            invalidate();
            return true;
        }
    }

    return false;
}

CWindow *CGridLayout::GetChild( size_t row, size_t col )
{
    BEATS_ASSERT(row < m_uRowCount && col < m_uColCount);

    return m_children[row*m_uColCount+col];
}

bool CGridLayout::RemoveChild( CWindow *window )
{
    auto itr = std::find(m_children.begin(), m_children.end(), window);
    if(itr != m_children.end())
    {
        *itr = nullptr;
        return true;
    }
    return false;
}

void CGridLayout::CalcSplitPos()
{
    kmScalar spaceWidth = m_fLeftMargin + m_fRightMargin + m_fHGap * (m_uColCount - 1);
    kmScalar averageWidth = (m_pWindow->RealSize().x - spaceWidth) / m_uColCount;
    kmScalar spaceHeight = m_fTopMargin + m_fBottomMargin + m_fVGap * (m_uRowCount - 1);
    kmScalar averageHeight = (m_pWindow->RealSize().y - spaceHeight) / m_uRowCount;

    kmScalar totalFixedWidth = 0.f;
    kmScalar totalFactoredWidth = 0.f;
    kmScalar totalFixedHeight = 0.f;
    kmScalar totalFactoredHeight = 0.f;
    for(auto width : m_everyColWidth)
    {
        totalFixedWidth += width.first;
        totalFactoredWidth += width.second;
    }
    for(auto height : m_everyRowHeight)
    {
        totalFixedHeight += height.first;
        totalFactoredHeight += height.second;
    }

    BEATS_ASSERT(totalFixedWidth <= m_pWindow->RealSize().x);
    BEATS_ASSERT(totalFixedHeight <= m_pWindow->RealSize().y);

    //vertical split position
    m_vSplitPos.clear();
    m_vSplitPos.resize(2*m_uRowCount);
    kmScalar y = m_fTopMargin;
    kmScalar yPercent = 0.f;
    for(size_t row = 0; row < m_uRowCount; ++row)
    {
        kmScalar height = m_fRowHeight;   //all row, same height
        kmScalar heightPercent = 0.f;
        if(height <= 0.f)   //all row, diffrent height
        {
            height = m_everyRowHeight[row].first;
            heightPercent = m_everyRowHeight[row].second;
            if(height > 0.f)    //fixed height
            {
                heightPercent = 0.f;
            }
            else if(heightPercent > 0.f)    //factored height
            {
                heightPercent = heightPercent / totalFactoredHeight;
                height = -(totalFixedHeight + spaceHeight) * heightPercent;
            }
            else    //average height
            {
                height = averageHeight;
            }
        }
        m_vSplitPos[2*row].first = y;
        m_vSplitPos[2*row].second = yPercent;
        m_vSplitPos[2*row+1].first = y + height;
        m_vSplitPos[2*row+1].second = yPercent + heightPercent;
        y += height + m_fVGap;
        yPercent += heightPercent;
    }

    //horizontal split position
    m_hSplitPos.clear();
    m_hSplitPos.resize(2*m_uColCount);
    kmScalar x = m_fLeftMargin;
    kmScalar xPercent = 0.f;
    for(size_t col = 0; col < m_uColCount; ++col)
    {
        kmScalar width = m_fColWidth;
        kmScalar widthPercent = 0.f;
        if(width <= 0.f)
        {
            width = m_everyColWidth[col].first;
            widthPercent = m_everyColWidth[col].second;
            if(width > 0.f) //fixed width
            {
                widthPercent = 0.f;
            }
            else if(widthPercent > 0.f) //factored width
            {
                widthPercent = widthPercent / totalFactoredWidth;
                width = -(totalFixedWidth + spaceWidth) * widthPercent;
            }
            else  //average width
            {
                width = averageWidth;
            }
        }
        m_hSplitPos[2*col].first = x;
        m_hSplitPos[2*col].second = xPercent;
        m_hSplitPos[2*col+1].first = x + width;
        m_hSplitPos[2*col+1].second = xPercent + widthPercent;
        x += width + m_fHGap;
        xPercent += widthPercent;
    }
}

void CGridLayout::PerformLayout()
{
    CalcSplitPos();

    //set position and size of child window
    for(size_t row = 0; row < m_uRowCount; ++row)
    {
        std::pair<kmScalar, kmScalar> top = m_vSplitPos[2*row];
        std::pair<kmScalar, kmScalar> bottom = m_vSplitPos[2*row+1];
        for(size_t col = 0; col < m_uColCount; ++col)
        {
            size_t index = row * m_uColCount + col;
            CWindow *child = m_children[index];
            if(child)
            {
                std::pair<kmScalar, kmScalar> left = m_hSplitPos[2*col];
                std::pair<kmScalar, kmScalar> right = m_hSplitPos[2*col+1];
                child->SetArea(left.first, top.first, left.second, top.second,
                    right.first, bottom.first, right.second, bottom.second);
            }
        }
    }

    CBaseLayout::PerformLayout();
}

