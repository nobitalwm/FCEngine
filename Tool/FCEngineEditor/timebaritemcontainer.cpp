#include "stdafx.h"
#include "timebaritemcontainer.h"
#include "timebarframe.h"
#include "timebarscale.h"
#include "timebaritem.h"

CTimeBarItemContainer::CTimeBarItemContainer(wxWindow* parent)
: wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL)
, m_iCellWidth(CELLWIDTH)
, m_pSyncWnd(NULL)
, m_pSyncScale(NULL)
, m_pSplitterwnd(NULL)
, m_iItemCellsCount(INITCELLNUM)
, m_iCursorPositionX(0)
{
    m_pMainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(m_pMainSizer);
    memset(&m_currentSelect, 0, sizeof(m_currentSelect));
    memset(&m_lastSelect, 0, sizeof(m_lastSelect));
    ShowScrollbars(wxSHOW_SB_ALWAYS,wxSHOW_SB_ALWAYS);
    SetScrollbars(CELLWIDTH, CELLHIGHT, SCROLLUNITSX, SCROLLUNITSY);
}

CTimeBarItemContainer::~CTimeBarItemContainer()
{

}

void CTimeBarItemContainer::SyncWith(CTimeBarDataViewCtrl *win, CTimeBarScale* win2)
{
    m_pSyncWnd = win;
    m_pSyncScale = win2;
}

void CTimeBarItemContainer::ScrollWindow(int dx, int dy, const wxRect *rect )
{
    super::ScrollWindow(dx, dy, rect);
    wxPoint ptViewStart = GetViewStart();
    bool bHorizontalMove = dx != 0 && dy == 0;
    if (bHorizontalMove)
    {
        wxPoint pt = ptViewStart;
        m_pSyncScale->Refresh(false);
    }
    else
    {
        m_pSyncWnd->Scroll(ptViewStart);
    }
}

void CTimeBarItemContainer::AddItem(CTimeBarItem* pItem)
{
    m_items.Add(pItem);
    m_pMainSizer->Add(pItem, 0, wxGROW|wxALL, 0);
    SetScrollbars(m_iCellWidth, CELLHIGHT, m_iItemCellsCount, m_items.GetCount());
}

void CTimeBarItemContainer::SetTimeBarFrameWindow(CTimeBarFrame* pSplitterWindow)
{
    m_pSplitterwnd = pSplitterWindow;
}

ArrayOfTimeBarItem* CTimeBarItemContainer::GetLineBarItems()
{
    return &m_items;
}

CTimeBarFrame* CTimeBarItemContainer::GetTimeBarFrameWindow()
{
    return m_pSplitterwnd;
}

SSelectCtrl& CTimeBarItemContainer::GetCurrentSelect()
{
    return m_currentSelect;
}

void CTimeBarItemContainer::SetCellWidth(int iWidth)
{
    m_iCellWidth = iWidth;
}

void CTimeBarItemContainer::DeleteItem(int index)
{
    CTimeBarItem* p = m_items.Detach(index);
    m_pMainSizer->Detach(p);
    delete p;
    m_pMainSizer->Layout();
}

void CTimeBarItemContainer::SelectItems()
{
    int columnstart = m_currentSelect.m_iSelectColumnBegin > m_currentSelect.m_iSelectColumnEnd ? m_currentSelect.m_iSelectColumnEnd : m_currentSelect.m_iSelectColumnBegin;
    int columnend = m_currentSelect.m_iSelectColumnBegin < m_currentSelect.m_iSelectColumnEnd ? m_currentSelect.m_iSelectColumnEnd : m_currentSelect.m_iSelectColumnBegin;
    int rowstart = m_currentSelect.m_iSelectRowBegin >= m_currentSelect.m_iSelectRowEnd ? m_currentSelect.m_iSelectRowEnd : m_currentSelect.m_iSelectRowBegin;
    int rowend = m_currentSelect.m_iSelectRowBegin <= m_currentSelect.m_iSelectRowEnd ? m_currentSelect.m_iSelectRowEnd : m_currentSelect.m_iSelectRowBegin;

    int num = m_items.GetCount();
    for (int i = 0; i < num; i++)
    {
        m_items[i].UnSelecteAll();
        if (i >= columnstart && i <= columnend)
        {
            m_items[i].SelectCells(rowstart,rowend);
        }
        m_items[i].Refresh(false);
    }
    m_lastSelect = m_currentSelect;
}

void CTimeBarItemContainer::SetItemCellsCount( int iCount )
{
    if (m_iItemCellsCount <= iCount)
    {
        m_iItemCellsCount = iCount + 100;
    } 
}

int CTimeBarItemContainer::GetCellWidth() const
{
    return m_iCellWidth;
}

void CTimeBarItemContainer::RefreshItems()
{
    int iCount = m_items.GetCount();
    for (int i = 0; i < iCount; i++)
    {
        m_items[i].Refresh(false);
    }
}
