#include "stdafx.h"
#include "timebarframe.h"
#include "wx/frame.h"
#include "wx/dcbuffer.h"
#include "wx/sizer.h"
#include "Render/Animation.h"
#include "DataViewCtrl.h"
#include "DataViewListModel.h"
#include "Utility/BeatsUtility/SharePtr.h"
#include "EditAnimationDialog.h"
#include "Render/AnimationController.h"
#include "AnimationGLWindow.h"

#define BUTTONSIZE 24
#define SASHPOSITION 130

BEGIN_EVENT_TABLE(CTimeBarFrame, wxSplitterWindow)
    EVT_SIZE(CTimeBarFrame::OnSize)
    EVT_SPLITTER_SASH_POS_CHANGED(wxID_ANY, CTimeBarFrame::OnPositionChanged)
    EVT_SPLITTER_SASH_POS_CHANGING(wxID_ANY, CTimeBarFrame::OnPositionChanging)
    EVT_SPINCTRL(wxID_ANY, CTimeBarFrame::OnSpinCtrl)
    EVT_TEXT_ENTER(wxID_ANY, CTimeBarFrame::OnSpinTextEnter)
    EVT_DATAVIEW_SELECTION_CHANGED(wxID_ANY, CTimeBarFrame::OnSelect)
END_EVENT_TABLE()

CTimeBarFrame::CTimeBarFrame(wxWindow *parent)
: wxSplitterWindow(parent,
                   wxID_ANY,
                   wxDefaultPosition, 
                   wxDefaultSize,
                   wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN)
, m_iCellWidth(CELLWIDTH)
, m_iCurrentSelect(INVALID_DATA)
{
    SetMinimumPaneSize(150);
    SetSashGravity(0.1);
    m_iCursorPositionX = 1;
    m_pFrame = parent;
    m_iItemId = 1;

    wxBoxSizer* rightpanelsizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* righttopsizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* leftpanelsizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* leftdownsizer = new wxBoxSizer(wxHORIZONTAL);
    wxPanel* rightpanelbase = new wxPanel(this);
    wxPanel* leftpanelbase = new wxPanel(this);
    wxStaticText* pText = new wxStaticText(leftpanelbase, wxID_ANY, _T("fps: "));
    wxPanel* pSpacePanel = new wxPanel(rightpanelbase, wxID_ANY, wxDefaultPosition, wxSize(DEFAULTSCROLLBARWIDTH, SCALEBARHIGHT));
    pSpacePanel->SetMinSize(wxSize(DEFAULTSCROLLBARWIDTH, SCALEBARHIGHT));
    pSpacePanel->SetMaxSize(wxSize(DEFAULTSCROLLBARWIDTH, SCALEBARHIGHT));
    m_pButtonAdd = new wxButton(leftpanelbase, wxID_ANY, "+");
    m_pButtonDelete = new wxButton(leftpanelbase, wxID_ANY, "-");
    m_pSpinCtrl = new wxSpinCtrl(leftpanelbase, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxSP_ARROW_KEYS|wxSP_VERTICAL|wxTE_PROCESS_ENTER, 0, MAXFPS, 0);

    m_pTimeLineDataViewCtrl = new CTimeBarDataViewCtrl(leftpanelbase, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLB_NO_SB);
    m_pTimeLineDataViewCtrl->AssociateListModel();
    m_pScalebar = new CTimeBarScale(rightpanelbase);
    m_pContainer = new CTimeBarItemContainer(rightpanelbase);
    m_pScalebar->SetTimeBarFrameWindow(this);
    m_pContainer->SetTimeBarFrameWindow(this);
    wxSize size(BUTTONSIZE, BUTTONSIZE);
    int iSpinCtrlWidth = BUTTONSIZE*2.5;
    m_pButtonAdd->SetMinSize(size);
    m_pButtonDelete->SetMinSize(size);
    m_pButtonDelete->SetMinSize(size);
    m_pSpinCtrl->SetMinSize(wxSize(iSpinCtrlWidth, BUTTONSIZE));
    SplitVertically(leftpanelbase, rightpanelbase, SASHPOSITION);
    rightpanelbase->SetSizer(rightpanelsizer);
    leftpanelbase->SetSizer(leftpanelsizer);
    rightpanelsizer->Add(righttopsizer, 0, wxGROW|wxALL, 0 );
    righttopsizer->Add(m_pScalebar, 1, wxGROW|wxALL, 0 );
    righttopsizer->Add(pSpacePanel, 0, wxGROW|wxALL, 0 );
    rightpanelsizer->Add(m_pContainer, 1, wxGROW|wxALL, 0 );
    leftpanelsizer->Add(m_pTimeLineDataViewCtrl, 1, wxGROW|wxALL, 0 );
    leftpanelsizer->Add(leftdownsizer, 0, wxGROW|wxALL, 0);
    leftdownsizer->Add(m_pButtonAdd, 0, wxGROW|wxALL, 0);
    leftdownsizer->Add(m_pButtonDelete, 0, wxGROW|wxALL, 0);
    leftdownsizer->AddStretchSpacer(1);
    leftdownsizer->Add(pText, 0, wxGROW|wxALL, 0);
    leftdownsizer->Add(m_pSpinCtrl, 0, wxGROW|wxALL, 0);

    m_pContainer->SyncWith(m_pTimeLineDataViewCtrl,m_pScalebar);
    m_pTimeLineDataViewCtrl->SyncWith(m_pContainer);

    m_pButtonAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CTimeBarFrame::OnBtnAddClick), NULL, this);
    m_pButtonDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CTimeBarFrame::OnBtnDeleteClick), NULL, this);
    SetFocus();
}

CTimeBarFrame::~CTimeBarFrame()
{

}

void CTimeBarFrame::OnBtnAddClick(wxCommandEvent& /*event*/)
{
    AddItems();
}

void CTimeBarFrame::OnBtnDeleteClick(wxCommandEvent& /*event*/)
{
    DeleteItems(m_iCurrentSelect);
    m_iCurrentSelect = -1;
}

void CTimeBarFrame::OnPositionChanged(wxSplitterEvent& event)
{
    ResetLeft();
    event.Skip();
}

void CTimeBarFrame::OnPositionChanging(wxSplitterEvent& event)
{
    ResetLeft();
    event.Skip();
}

void CTimeBarFrame::OnSpinCtrl(wxSpinEvent& event)
{
    int iData = event.GetValue();
    SetFpsToAnimation(iData);
}

void CTimeBarFrame::OnSpinTextEnter(wxCommandEvent& event)
{
    unsigned long iValue  = 0;
    if (event.GetString().ToCULong(&iValue))
    {
        SetFpsToAnimation(iValue);
    }
}

void CTimeBarFrame::OnSize(wxSizeEvent & event)
{
    super::OnSize(event);
    ResetLeft();
    GetWindow1()->Refresh(false);
}

void CTimeBarFrame::ResetLeft()
{
    m_pTimeLineDataViewCtrl->RefreshSize();
}

void CTimeBarFrame::AddItems()
{
    int iCount = m_pTimeLineDataViewCtrl->GetUserListModel()->GetCount();
    wxVariant Variant;
    wxString strCurrentName;
    wxString strNewName = "new item 0";
    for (int j = 0; j < iCount; j++)
    {
        strNewName = wxString::Format(_T("new item %d"),m_iItemId);
        for (int i = 0; i < iCount; i++)
        {
            m_pTimeLineDataViewCtrl->GetUserListModel()->GetValueByRow(Variant, i, 0);
            strCurrentName = Variant.GetString();
            if (strCurrentName == strNewName)
            {
                m_iItemId++;
                strNewName = wxString::Format(_T("new item %d"),m_iItemId);
                break;
            }
        }
    }
    m_pTimeLineDataViewCtrl->AddItem(strNewName, true, false);
    m_pContainer->AddItem(new CTimeBarItem(m_pContainer));
}

void CTimeBarFrame::AddItems( SharePtr<CAnimation> pAnimationItem )
{
    CTimeBarItem* pItem = new CTimeBarItem(m_pContainer);
    int iCount = pAnimationItem->GetFrameCount();
    pItem->SetCellsDataRange(0, iCount - 1);
    m_pContainer->SetItemCellsCount(iCount);
    m_pContainer->AddItem(pItem);
    wxString name = pAnimationItem->GetFilePath();
    m_pTimeLineDataViewCtrl->AddItem(name.AfterLast('\\'), true, false);
}

void CTimeBarFrame::DeleteItems(int index)
{
    if (index != -1)
    {
        ((CTimeBarListModel*)m_pTimeLineDataViewCtrl->GetModel())->DeleteItem(index);
        m_pContainer->DeleteItem(index);
    }
}

void CTimeBarFrame::ClickOnScaleBar()
{
    if (m_bclickonscalebar)
    {
        m_pContainer->GetCurrentSelect().m_iSelectColumnBegin = -1;
        m_pContainer->GetCurrentSelect().m_iSelectColumnEnd = -1;
        m_pContainer->GetCurrentSelect().m_iSelectRowBegin = -1;
        m_pContainer->GetCurrentSelect().m_iSelectRowEnd = -1;
    }

    m_pContainer->SelectItems();
    m_pScalebar->Refresh(false);
}

void CTimeBarFrame::SetCursorPositionX(int pos)
{
    CEditAnimationDialog* pDialog = (CEditAnimationDialog*)GetFrameWindow();
    m_iCursorPositionX = pos < 0 ? 0 : pos;
    int iIndex = m_iCursorPositionX - m_iCellWidth * 0.5F;
        iIndex = iIndex / m_iCellWidth;
    m_pContainer->SelectItems();
    if (pDialog->IsLoadAllAnimationResource())
    {
        if (iIndex < pDialog->GetAnimation()->GetFrameCount())
        {
            pDialog->GetCanvas()->GetModel()->GetAnimationController()->GoToFrame(iIndex);
        }
    }
    m_pScalebar->Refresh(false);
}

void CTimeBarFrame::SetCurrentSelect(int iCurrentSelect)
{
    m_iCurrentSelect = iCurrentSelect;
}

int CTimeBarFrame::GetCurrentSelect() const
{
    return m_iCurrentSelect;
}

CTimeBarScale* CTimeBarFrame:: GetScalebar()
{
    return m_pScalebar;
}

void CTimeBarFrame::SetClickOnScalebar(bool bIsClicked)
{
    m_bclickonscalebar = bIsClicked;
}

bool CTimeBarFrame::IsClickOnScalebar() const
{
    return m_bclickonscalebar;
}

CTimeBarItemContainer* CTimeBarFrame::GetItemContainer()
{
    return m_pContainer;
}

void CTimeBarFrame::SetFps( size_t iFps )
{
    m_pSpinCtrl->SetValue(iFps);
}

void CTimeBarFrame::SetFpsToAnimation( size_t /*iFps*/ )
{
    //TODO: Set FPS
}

void CTimeBarFrame::SetFrameWindow( wxWindow* pWindow )
{
    m_pFrame = pWindow;
}

wxWindow* CTimeBarFrame::GetFrameWindow()
{
    return m_pFrame;
}

void CTimeBarFrame::SetCurrentCursor( int pos )
{
    wxPoint point = m_pContainer->GetViewStart();
    m_iCursorPositionX = (pos + 0.5F) * m_iCellWidth;
    int iRange = m_pContainer->GetSize().x / m_pContainer->GetCellWidth();
    
    if (pos >= iRange + point.x)
    {
        point.x += pos - point.x - 10;
        m_pContainer->Scroll(point);
    }
    else if (pos < point.x)
    {
        point.x = pos;
        m_pContainer->Scroll(point);
    }
    m_pContainer->RefreshItems();
    m_pScalebar->Refresh(false);
}

int CTimeBarFrame::GetCursorPositionX()
{
    return m_iCursorPositionX;
}

int CTimeBarFrame::GetItemCount()
{
    return ((CTimeBarListModel*)m_pTimeLineDataViewCtrl->GetModel())->GetCount();
}

void CTimeBarFrame::OnSelect(wxDataViewEvent& event)
{
    wxDataViewItem item = event.GetItem();
    int row = (int)item.GetID() - 1;
    SetCurrentSelect(row);
}

int CTimeBarFrame::GetCellWidth()
{
    return m_iCellWidth;
}
