#include "stdafx.h"
#include "ListControl.h"
#include "GUI/Event/WindowEvent.h"
#include "Event/MouseEvent.h"
#include "GUI/WindowManager.h"
#include "GUI/Layout/GridLayout.h"

using namespace FCGUI;

CListControl::CListControl()
{
    Init();
}

CListControl::CListControl(const TString &name)
    : CWindow(name)
{
    Init();
}

CListControl::~CListControl()
{

}

void CListControl::ReflectData(CSerializer& serializer)
{
    CWindow::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_fRowHeight, true, 0xFFFFFFFF, _T("行高度"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_fColWidth, true, 0xFFFFFFFF, _T("列宽度"), NULL, NULL, NULL);
}

bool CListControl::OnMouseEvent( CMouseEvent *event )
{
    bool bRet = false;
    if(IsEnabled())
    {
        if(event->Button() == GLFW_MOUSE_BUTTON_LEFT)
        {
            if(event->Type() == eEVT_EVENT_MOUSE_PRESSED)
            {
                m_bPressed = true;
                kmVec2Fill(&m_pressedPos, event->X(), event->Y());
            }
            else if(event->Type() == eEVT_EVENT_MOUSE_RELEASED)
            {
                m_bPressed = false;
            }
            else if(event->Type() == eEVT_EVENT_MOUSE_MOVED)
            {
                if(m_bPressed)
                {
                    kmVec2 currPos;
                    kmVec2Fill(&currPos, event->X(), event->Y());
                    kmVec2 offset;
                    kmVec2Fill(&offset, currPos.x - m_pressedPos.x , currPos.y - m_pressedPos.y);
                    CheckScrollOffset(offset);
                    if(!BEATS_FLOAT_EQUAL(offset.x, 0) || !BEATS_FLOAT_EQUAL(offset.y, 0))
                    {
                        Scroll(offset);
                        WindowEvent event(EVENT_MOVE);
                        DispatchEvent(&event);
                    }
                }
            }
        }
        bRet = true;
    }
    return bRet;
}

void CListControl::AddItem(CWindow *item)
{
    CGridLayout *layout = static_cast<CGridLayout*>(Layout());
    layout->AddChild(item);
    CalcScrollOffsetRange();
}

void CListControl::RemoveItem(CWindow *item)
{
    CGridLayout *layout = static_cast<CGridLayout*>(Layout());
    layout->RemoveChild(item);
}

CWindow* CListControl::GetItem(size_t row, size_t col)
{
    CGridLayout *layout = static_cast<CGridLayout*>(Layout());
    return layout->GetChild(row, col);
}

void CListControl::SetRowHeight(kmScalar rowHeight)
{
     CGridLayout *layout = static_cast<CGridLayout*>(Layout());
     m_fRowHeight = rowHeight;
     layout->SetRowHeight(m_fRowHeight);
}

void CListControl::SetColWidth(kmScalar colWidth)
{
    CGridLayout *layout = static_cast<CGridLayout*>(Layout());
    m_fColWidth = colWidth;
    layout->SetColWidth(colWidth);
}

void CListControl::Init()
{
    kmVec2Fill(&m_scrollOfffsetRange, 0.f, 0.f);
    kmVec2Fill(&m_scrollOffset, 0.f, 0.f);
    m_fRowHeight = 20.f;
    m_fColWidth =20.f;
    m_fTopMargin = 10.f;
    m_fBottomMargin = 10.f;
    m_fLeftMargin = 10.f;
    m_fRightMargin = 10.f;
    m_fHGap = 10.f;
    m_fVGap = 10.f;
    InitLayout();
}

void CListControl::InitLayout()
{
    CGridLayout *pGridLayout = new CGridLayout(2, 2);
    pGridLayout->SetMargin(m_fTopMargin, m_fBottomMargin, m_fLeftMargin, m_fRightMargin);
    pGridLayout->SetGap(m_fHGap, m_fVGap);
    pGridLayout->SetRowHeight(m_fRowHeight);
    pGridLayout->SetColWidth(m_fColWidth);
    SetLayout(pGridLayout);
    CalcScrollOffsetRange();
}

void CListControl::CheckScrollOffset(kmVec2 &offset)
{
    m_scrollOffset.x += offset.x;
    m_scrollOffset.y += offset.y;
    if( m_scrollOffset.x < 0 )
    {
        offset.x = offset.x - m_scrollOffset.x;
        m_scrollOffset.x = 0;
    }
    else if(m_scrollOffset.x > m_scrollOfffsetRange.x)
    {
        offset.x = offset.x - (m_scrollOffset.x - m_scrollOfffsetRange.x);
        m_scrollOffset.x = m_scrollOfffsetRange.x;
    }
    if( m_scrollOffset.y < 0 )
    {
        offset.y = offset.y - m_scrollOffset.y;
        m_scrollOffset.y = 0;
    }
    else if(m_scrollOffset.y > m_scrollOfffsetRange.y)
    {
        offset.y = offset.y - (m_scrollOffset.y - m_scrollOfffsetRange.y);
        m_scrollOffset.y = m_scrollOfffsetRange.y;
    }
}

void CListControl::CalcScrollOffsetRange()
{
    kmVec2 viewSize = RealSize();
    CGridLayout *layout = static_cast<CGridLayout*>(Layout());
    kmScalar realWidth = 0;
    kmScalar realHeight = 0;
    size_t row = layout->RowCount();
    size_t col = layout->ColCount();

    realWidth +=  (col - 1) * ((layout->LeftMargin() + layout->RightMargin()) / 2.0f );
    realWidth +=  col * layout->HGap();
    realWidth += m_fColWidth * col;

    realHeight += ( row - 1) *((layout->TopMargin() + layout->BottomMargin()) / 2.0f);
    realHeight += row * layout->VGap();
    realHeight += m_fRowHeight * row;

    m_scrollOfffsetRange.x = realWidth - viewSize.x;
    m_scrollOfffsetRange.y = realHeight - viewSize.y;
}
