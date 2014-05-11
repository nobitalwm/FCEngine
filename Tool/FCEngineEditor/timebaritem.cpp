#include "stdafx.h"
#include <wx/dcbuffer.h>
#include "timebaritem.h"
#include "timebaritemcontainer.h"
#include "timebarframe.h"
#include "EditAnimationDialog.h"
#include "Render/AnimationController.h"

#define DATACELLHIGHT 18
#define NODATACELLHIGHT 24
#define DATACELLOFFSET 3
#define WIDTHADJUSTMENT 1
#define POSITIONADJUSTMENT -1

ArrayOfTimeBarItem::~ArrayOfTimeBarItem()
{

}

CTimeBarItem::CTimeBarItem( wxWindow *parent, wxWindowID id, 
                             const wxPoint &pos, const wxSize &size, 
                             long style, const wxString &name )
                             : wxPanel(parent, id, pos, size, style , name),
                             m_iDataBegin(INVALID_DATA),
                             m_iDataEnd(INVALID_DATA)
{
    SetMinSize(wxSize(0, CELLHIGHT));
}

CTimeBarItem::~CTimeBarItem()
{
}

BEGIN_EVENT_TABLE(CTimeBarItem, wxPanel)
    EVT_PAINT(CTimeBarItem::OnPaint)
    EVT_MOUSE_EVENTS(CTimeBarItem::OnMouse)
    EVT_MOUSE_CAPTURE_LOST(CTimeBarItem::OnMouseCaptureLost)
END_EVENT_TABLE()

void CTimeBarItem::DrawItem()
{
    static const size_t GrayCellOffset = 5;
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    wxAutoBufferedPaintDC ScaleDC(this); 
    wxRect ItemRect = GetRect();
    CTimeBarItemContainer* pContainer = (CTimeBarItemContainer*)GetParent(); 
    wxPoint ViewStart = pContainer->GetViewStart();
    wxSize size = pContainer->GetSize();
    int cursorpositionx = pContainer->GetTimeBarFrameWindow()->GetCursorPositionX();
    int iWidith = pContainer->GetCellWidth();
    int iCount  = size.x / iWidith;
    iCount += ViewStart.x - 1;
    int iDrawRectWdith = iWidith +1;
    int iPositionX = -1;
    int iDefaultCellHeight = ItemRect.height + 1;
    int iCellDataHeight = ItemRect.height - 2 * DATACELLOFFSET;

    ScaleDC.SetPen(*wxLIGHT_GREY);
    ScaleDC.DrawRectangle(ItemRect);
    for(int i = ViewStart.x; i < iCount; i++)
    {
        iPositionX = i * iWidith;
        bool bHasData = HasData(i);
        bool bHasSelected = HasSelected(i);
        if (!bHasData)
        {
            if (bHasSelected)
            {
                ScaleDC.SetBrush(*wxBLUE);
                ScaleDC.DrawRectangle(iPositionX, 0, iDrawRectWdith, iDefaultCellHeight);
                ScaleDC.SetBrush(*wxWHITE);
            }
            else
            {
                if (i % GrayCellOffset == 0)
                {
                    ScaleDC.SetBrush(*wxLIGHT_GREY);
                    ScaleDC.DrawRectangle(iPositionX, 0, iDrawRectWdith, iDefaultCellHeight);
                    ScaleDC.SetBrush(*wxWHITE);
                }
                else
                {
                    ScaleDC.DrawRectangle(iPositionX, 0, iDrawRectWdith, iDefaultCellHeight);
                }
            }
            
        }
        else//if(bHasData)
        {
            if (bHasSelected)
            {
                DrawDefaultItem(&ScaleDC, i, iPositionX, iDrawRectWdith, iDefaultCellHeight);
                ScaleDC.SetBrush(*wxBLUE);
                ScaleDC.DrawRectangle(iPositionX, DATACELLOFFSET, iDrawRectWdith, iCellDataHeight);
                ScaleDC.SetBrush(*wxWHITE);
            }
            else
            {
                DrawDefaultItem(&ScaleDC, i, iPositionX, iDrawRectWdith, iDefaultCellHeight);
                ScaleDC.SetBrush(*wxGREEN);
                ScaleDC.DrawRectangle(iPositionX, DATACELLOFFSET, iDrawRectWdith, iCellDataHeight);
                ScaleDC.SetBrush(*wxWHITE);
            }
        }
    }
    wxPen pen = *wxRED_PEN;
    pen.SetWidth(CURSORWIDTH);
    ScaleDC.SetPen(pen);
    ScaleDC.DrawLine(cursorpositionx, 0, cursorpositionx, ItemRect.height);
}

void CTimeBarItem::OnMouseCaptureLost(wxMouseCaptureLostEvent& /*event*/)
{
    UnSelecteAll();
}

void CTimeBarItem::OnPaint(wxPaintEvent& /*event*/)
{
    DrawItem();
}

int CTimeBarItem::GetColumnID()
{
    int iRet = -1;
    CTimeBarItemContainer* pParent = (CTimeBarItemContainer*)GetParent();
    int num = pParent->GetLineBarItems()->GetCount();
    for (int i = 0; i < num; i++)
    {
        if (&(pParent->GetLineBarItems()->Item(i)) == this)
        {
            iRet = i;
            break;
        }
    }
    return iRet;
}

void CTimeBarItem::OnMouse(wxMouseEvent& event)
{
    
    CTimeBarItemContainer* pParent = (CTimeBarItemContainer*)GetParent();
    CTimeBarFrame* pFrame = pParent->GetTimeBarFrameWindow();
    wxPoint pstn= event.GetPosition(); 
    int index = PointToCell(pstn);
    int iCellWidth = pParent->GetCellWidth();
    int CursorPosition = pstn.x / iCellWidth;
    CursorPosition *= iCellWidth;
    CursorPosition += iCellWidth * 0.5F;
    if (event.ButtonDown(wxMOUSE_BTN_LEFT))
    {
        pFrame->SetClickOnScalebar(false);
        pParent->GetCurrentSelect().m_iSelectRowBegin = index;
        pParent->GetCurrentSelect().m_iSelectColumnBegin = GetColumnID();
        pParent->GetCurrentSelect().m_iSelectRowEnd = index;
        pParent->GetCurrentSelect().m_iSelectColumnEnd = GetColumnID();
        pFrame->SetCursorPositionX(CursorPosition);
        pFrame->ClickOnScaleBar();
    }
    else if (event.Dragging())
    {   
        if (pstn.x % FREQUENCYREDUCTIONFACTOR == 0)
        {
            if (!pParent->GetTimeBarFrameWindow()->IsClickOnScalebar())
            {
                pParent->GetCurrentSelect().m_iSelectRowEnd = index;
                pParent->GetCurrentSelect().m_iSelectColumnEnd = GetColumnID();
            }
            pFrame->SetCursorPositionX(CursorPosition);
            pFrame->ClickOnScaleBar();
        }
    }
    else if (event.ButtonUp(wxMOUSE_BTN_LEFT))
    {
        int id = GetColumnID();
        pFrame->SetCurrentSelect(id);
    }
    event.Skip();
};

int CTimeBarItem::PointToCell(wxPoint point)
{
    CTimeBarItemContainer* pParent = (CTimeBarItemContainer*)GetParent();
    return point.x / pParent->GetCellWidth();
}

wxPoint CTimeBarItem::CellToPoint(int index)
{
    CTimeBarItemContainer* pParent = (CTimeBarItemContainer*)GetParent();
    return wxPoint(index * pParent->GetCellWidth(), 0);
}

void CTimeBarItem::UnSelecteAll()
{
    m_iSelectionEnd = INVALID_DATA;
}

void CTimeBarItem::SelectCell( int index, bool /*bselect*/ )
{
    if (HasSelected(index))
    {
        SelectCells(index, index);
    }
}

void CTimeBarItem::SelectCells(int start, int end)
{
    m_iSelectionBegin = start > end ? end : start;
    m_iSelectionEnd = start < end ? end : start;
}

wxColour CTimeBarItem::GetColour(int ColourID)
{
    wxColour color = *wxWHITE;
    switch (ColourID)
    {
    case eIC_BACKGROUNDCOLOUR2:
        color = *wxLIGHT_GREY;
        break;

    case eIC_DATACELLCOLOUR:
        color = *wxGREEN;
        break;

    case eIC_SELECTEDCOLOUR:
        color = *wxBLUE;
        break;

    case eIC_BORDERCOLOUR:
        color = *wxLIGHT_GREY;
        break;

    case eIC_BACKGROUNDCOLOUR1:
    default:
        break;
    }

    return color;
}

bool CTimeBarItem::HasData( int iFrameIndex )
{
    bool bRet = iFrameIndex >= m_iDataBegin && iFrameIndex <= m_iDataEnd ;
    return bRet;
}

bool CTimeBarItem::HasSelected( int iFrameIndex )
{
    bool bRet = iFrameIndex >= m_iSelectionBegin && iFrameIndex <= m_iSelectionEnd;
    return bRet;
}

void CTimeBarItem::SetCellsDataRange( int iDataBegin, int iDataEnd )
{
    m_iDataBegin = iDataBegin > iDataEnd ? iDataEnd : iDataBegin;
    m_iDataEnd = iDataEnd > iDataBegin ? iDataEnd : iDataBegin;
}

void CTimeBarItem::DrawDefaultItem( wxDC* pDC, int iIndex, int iPositionX, int iWidth, int iHeight )
{
    static const size_t GrayCellOffset = 5;
    if (iIndex % GrayCellOffset == 0)
    {
        pDC->SetBrush(*wxLIGHT_GREY);
        pDC->DrawRectangle(iPositionX, 0, iWidth, iHeight);
        pDC->SetBrush(*wxWHITE);
    }
    else
    {
        pDC->DrawRectangle(iPositionX, 0, iWidth, iHeight);
    }
}
