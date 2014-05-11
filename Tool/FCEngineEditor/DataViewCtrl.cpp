#include "stdafx.h"
#include "DataViewCtrl.h"
#include "timebarframe.h"
#include "DataViewListModel.h"
#include "DataViewCheckRender.h"
#include <wx/dcbuffer.h>
#include "DataViewTreeModelNode.h"
#include "DataViewTreeModel.h"

#define COLUMNWIDTH 25

BEGIN_EVENT_TABLE(CTimeBarDataViewCtrl, wxDataViewCtrl)
EVT_SCROLLWIN(CTimeBarDataViewCtrl::OnScroll)
END_EVENT_TABLE()

ArrayOfAnimation::~ArrayOfAnimation()
{

}

CTimeBarDataViewCtrl::CTimeBarDataViewCtrl( wxWindow *parent, wxWindowID id,
                     const wxPoint& pos,const wxSize& size, long style,
                     const wxValidator& validator,const wxString& name)
                     : wxDataViewCtrl(parent, id, pos, size, style, validator, name)
                     , m_pSyncWnd(NULL)
                     , m_pListModel(NULL)
                     , m_pTreeModel(NULL)
{
}

CTimeBarDataViewCtrl::~CTimeBarDataViewCtrl()
{
    AssociateModel(NULL);
    BEATS_SAFE_DELETE(m_pListModel);
    BEATS_SAFE_DELETE(m_pTreeModel);
}

int CTimeBarDataViewCtrl::GetRow(const wxDataViewItem & item)
{
    return GetRowByItem(item);
}

void CTimeBarDataViewCtrl::SyncWith(wxScrollHelperBase *win)
{
    m_pSyncWnd = win;
}

void CTimeBarDataViewCtrl::AddItem( const wxString &text, bool visible, bool islock )
{
    BEATS_ASSERT(m_pListModel)
    m_pListModel->AddItem(text, visible, islock);
}

bool CTimeBarDataViewCtrl::InitColumn()
{
    ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);
    AppendTextColumn(_T("Layer"),
        Col_EditableText,
        wxDATAVIEW_CELL_EDITABLE,
        wxCOL_WIDTH_AUTOSIZE);

    AppendColumn(
        new wxDataViewColumn(_T("V"),
        new CTimeBarCheckRenderer(Col_Visible),
        Col_Visible,
        COLUMNWIDTH,
        wxALIGN_CENTER,
        0)
        );

    AppendColumn(
        new wxDataViewColumn(_T("L"),
        new CTimeBarCheckRenderer(Col_Lock),
        Col_Lock,
        COLUMNWIDTH,
        wxALIGN_CENTER,
        0)
        );

    return true;
}

void CTimeBarDataViewCtrl::OnScroll(wxScrollWinEvent& /*event*/)
{
    if (m_pSyncWnd != NULL)
    {
        m_pSyncWnd->Scroll(GetViewStart());
    }
}
void CTimeBarDataViewCtrl::RefreshSize()
{
    int iRestFixedWidth = 0;
    wxSize curSize = GetSize();
    int iColumnCount = GetColumnCount();
    for (int i = 1; i < iColumnCount; i++)
    {
        iRestFixedWidth += GetColumn(i)->GetWidth();
    }
    GetColumn(0)->SetWidth(curSize.GetWidth() - iRestFixedWidth);
}

void CTimeBarDataViewCtrl::AppendItem( CDataViewTreeModelNode* pParent, const wxString &text, bool bVisible, bool bIslock )
{
    BEATS_ASSERT(m_pTreeModel)
    if (pParent == NULL)
    {
        pParent = m_pTreeModel->GetRootNode();
    }
    CDataViewTreeModelNode* pChild = new CDataViewTreeModelNode(pParent, text, bVisible, bIslock);
    pParent->Append(pChild);
    wxDataViewItem child( (void*) pChild );
    wxDataViewItem parent( (void*) pParent );
    m_pTreeModel->ItemAdded(parent, child);
    if (parent.IsOk())
    {
        Expand(parent);
    }
}

CTimeBarListModel* CTimeBarDataViewCtrl:: GetUserListModel()const
{
    return m_pListModel;
}

CDataViewTreeModel* CTimeBarDataViewCtrl::GetUserTreeModel() const
{
    return m_pTreeModel;
}

void CTimeBarDataViewCtrl::AssociateListModel()
{
    BEATS_ASSERT(m_pListModel == NULL)
    m_pListModel = new CTimeBarListModel();
    AssociateModel(m_pListModel);
    InitColumn();
}

void CTimeBarDataViewCtrl::AssociateTreeModel()
{
    BEATS_ASSERT(m_pTreeModel == NULL)
    m_pTreeModel = new CDataViewTreeModel();
    m_pTreeModel->SetAssociateCtrl(this);
    AssociateModel(m_pTreeModel);
    InitColumn();
}
