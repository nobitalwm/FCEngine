#include "stdafx.h"
#include "DataViewCheckRender.h"
#include "DataViewListModel.h"
#include "timebarevent.h"
#include "DataViewCtrl.h"
#include "DataViewTreeModel.h"

#define RADIUS 3
#define DECREASECOORD 2
#define ICONSIZE 20
CTimeBarCheckRenderer::CTimeBarCheckRenderer(int type)
                : wxDataViewCustomRenderer(_T("string"), wxDATAVIEW_CELL_ACTIVATABLE, wxALIGN_CENTER)
                , m_iconRect(wxSize(ICONSIZE,ICONSIZE))
                , m_nType(type)
{

}

CTimeBarCheckRenderer::~CTimeBarCheckRenderer()
{

}

bool CTimeBarCheckRenderer::Render( wxRect rect, wxDC *dc, int state )
{
    dc->SetBrush( *wxLIGHT_GREY_BRUSH );
    dc->SetPen( *wxTRANSPARENT_PEN );

    rect.Deflate(DECREASECOORD);
    dc->DrawRoundedRectangle( rect, RADIUS );

    RenderText(m_strValue,
                0, // no offset
                wxRect(dc->GetTextExtent(m_strValue)).CentreIn(rect),
                dc,
                state);
    return true;
}

bool CTimeBarCheckRenderer::ActivateCell(const wxRect& WXUNUSED(cell),
                                      wxDataViewModel *model,
                                      const wxDataViewItem &item,
                                      unsigned int col,
                                      const wxMouseEvent *mouseEvent)
{
    bool bSelected = GetOwner()->GetOwner()->IsSelected(item);
    bool bExamParameter = mouseEvent->m_x < 0 || mouseEvent->m_y < 0 || mouseEvent->m_x > m_iconRect.width || mouseEvent->m_y > m_iconRect.height;
    if (bSelected)
    {
        if (!bExamParameter)
        {
            wxVariant variant;
            CTimeBarEvent event(TIMEBAR_EVENT_TYPE, 0);
            CTimeBarDataViewCtrl* pDataViewCtrl = (CTimeBarDataViewCtrl*)GetOwner()->GetOwner();
            if (pDataViewCtrl->GetUserListModel())
            {
                CTimeBarListModel* pMyModel = (CTimeBarListModel*)model;
                int row = pMyModel->GetRow(item);
                variant = m_strValue == _T("-") ? (char)true : (char)false;
                pMyModel->SetValueByRow( variant, row, col );
                event.SetRow(row);
                event.SetType(m_nType);
            }
            else
            {
                CDataViewTreeModel* pMyModel = (CDataViewTreeModel*)model;
                wxDataViewItem tmpitem = item;
                variant = m_strValue == _T("-") ? (char)true : (char)false;
                pMyModel->SetValue(variant, item, col);
                event.SetItem(tmpitem);
            }
            pDataViewCtrl->GetEventHandler()->ProcessEvent(event);
        }
    }
    return bExamParameter;
}

wxSize CTimeBarCheckRenderer::GetSize() const
{
    return m_iconRect.GetSize();
}

bool CTimeBarCheckRenderer::SetValue( const wxVariant &value )
{
    bool bRet = value.IsNull();
    if (!bRet)
    {
        m_strValue = value.GetChar() == 0 ? _T("-") : _T("+");
    }
    return bRet;
}

bool CTimeBarCheckRenderer::GetValue( wxVariant &WXUNUSED(value) ) const
{
    return true;
}