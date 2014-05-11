#ifndef __TIMELINEBARITEM_H
#define __TIMELINEBARITEM_H

#define CELLWIDTH 10

#include "wx/panel.h"

class CTimeBarItem : public wxPanel
{
    typedef wxPanel super;
public:
    enum
    {
        Cell_Type0,
        Cell_Type1,
        Cell_Type2,
        Cell_Type3
    };
    enum EItemColor
    {
        eIC_BACKGROUNDCOLOUR1,
        eIC_BACKGROUNDCOLOUR2,
        eIC_BORDERCOLOUR,
        eIC_DATACELLCOLOUR,
        eIC_SELECTEDCOLOUR
    };
    CTimeBarItem( wxWindow *parent, wxWindowID id=wxID_ANY, 
        const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, 
        long style= wxTAB_TRAVERSAL, const wxString &name=wxPanelNameStr );
    ~CTimeBarItem();
    void        DrawItem();
    void        UnSelecteAll();
    void        SelectCell( int index, bool bselect );
    void        SelectCells(int start, int end = -1);
    int         PointToCell(wxPoint point);
    wxPoint     CellToPoint(int index);
    void        OnMouseCaptureLost(wxMouseCaptureLostEvent& event);
    void        OnPaint( wxPaintEvent& event );
    void        OnMouse(wxMouseEvent& event);
    int         GetColumnID();
    void        DrawDefaultItem(wxDC* pDC, int iIndex, int iPositionX, int iWidth, int iHeight);
    void        SetCellsDataRange(int iDataBegin, int iDataEnd);
    wxColour    GetColour(int ColourID);
    bool        HasSelected(int iFrameIndex);
    bool        HasData(int iFrameIndex);

private:
    int         m_iSelectionBegin;
    int         m_iSelectionEnd;
    int         m_iDataBegin;
    int         m_iDataEnd;
    DECLARE_EVENT_TABLE()
};
WX_DECLARE_OBJARRAY(CTimeBarItem, ArrayOfTimeBarItem);

#endif