#include "stdafx.h"
#include "EditorMainFrame.h"
#include "ConstantCurveProperty.h"
#include "ConstantCurveEditor.h"
#include "EditAnimationDialog.h"
#include "timebarframe.h"
#include "FCEditorGLWindow.h"
#include "EngineEditor.h"
#include "ComponentFileTreeItemData.h"
#include "FCEditorComponentWindow.h"
#include "Utility/BeatsUtility/UtilityManager.h"
#include "ComponentTreeItemData.h"
#include "Utility/TinyXML/tinyxml.h"
#include "ComponentGraphics_GL.h"
#include "Resource/ResourcePathManager.h"
#include "EditLanguageDialog.h"
#include "EditUIDialog.h"
#include "EditPerformanceDialog.h"
#include "Render/Camera.h"
#include "EngineCenter.h"
#include "CommandManager.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentProxyManager.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentEditorProxy.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentProject.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentGraphic.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentProjectData.h"
#include "Utility/BeatsUtility/ComponentSystem/ComponentPublic.h"
#include "Utility/BeatsUtility/StringHelper.h"

#include <wx/tglbtn.h>
#include <wx/artprov.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/numdlg.h>
#include <wx/srchctrl.h>
#include <wx/timer.h>
#include <Shlwapi.h>

#define MAINFRAMENORMALSIZE wxSize(1024, 800)
#define MAINFRAMEPOSITION wxPoint(40, 40)
#define BUTTONSIZE wxSize(20, 20)
#define SIZERBORDERWIDTH 5
#define IDBEGIN 1111

enum 
{
    PARTICLE_SIMULATION_WORLD = 1,
    PARTICLE_SIMULATION_LOCAL
};

enum EMenuID
{
    eMB_File,
    eMB_Edit,
    eMB_Assets,
    eMB_GameObj,
    eMB_Conponent,
    eMB_Window,
    eMB_Help
};

class CComponentInstanceTreeItemData : public wxTreeItemData
{
public:
    CComponentInstanceTreeItemData(CComponentBase* pComponentBase)
        : m_pComponentBase(pComponentBase)
    {

    }
    virtual ~CComponentInstanceTreeItemData()
    {

    }

    CComponentBase* GetComponentBase() const
    {
        return m_pComponentBase;
    }
private:
    CComponentBase* m_pComponentBase;
};


BEGIN_EVENT_TABLE(CEditorMainFrame, wxFrame)
EVT_COMMAND(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, CEditorMainFrame::OnAuiButton)
EVT_TREE_ITEM_ACTIVATED(Ctrl_Tree_CompontentFile, CEditorMainFrame::OnActivateComponentFile)
EVT_TREE_BEGIN_DRAG(Ctrl_Tree_CompontentFile,CEditorMainFrame::OnComponentFileStartDrag)
EVT_TREE_END_DRAG(Ctrl_Tree_CompontentFile,CEditorMainFrame::OnComponentFileEndDrag)
EVT_TREE_ITEM_RIGHT_CLICK(Ctrl_Tree_CompontentFile,CEditorMainFrame::OnRightClickComponentFileList)
EVT_TREE_BEGIN_DRAG(Ctrl_Tree_Compontent,CEditorMainFrame::OnComponentStartDrag)
EVT_TREE_END_DRAG(Ctrl_Tree_Compontent,CEditorMainFrame::OnComponentEndDrag)
EVT_TREE_SEL_CHANGED(Ctrl_Tree_Compontent,CEditorMainFrame::OnTemplateComponentItemChanged)
EVT_SEARCHCTRL_SEARCH_BTN(wxID_ANY, CEditorMainFrame::OnSearch)
EVT_SPLITTER_DCLICK(wxID_ANY, CEditorMainFrame::OnSplitterSashDClick)
EVT_SPLITTER_SASH_POS_CHANGING(wxID_ANY, CEditorMainFrame::OnSplitterSashDClick)
EVT_TEXT_ENTER(wxID_ANY, CEditorMainFrame::OnSearch)
EVT_TEXT(Ctrl_Search_TreeSearch, CEditorMainFrame::OnSearchTextUpdate)
EVT_IDLE(CEditorMainFrame::OnIdle)
EVT_TIMER(wxID_ANY, CEditorMainFrame::OnTimer)
EVT_TREE_ITEM_EXPANDED(wxID_ANY, CEditorMainFrame::OnTreeCtrlExpanded)
EVT_TREE_ITEM_COLLAPSED(wxID_ANY, CEditorMainFrame::OnTreeCtrlCollapsed)
EVT_TREE_ITEM_ACTIVATED(Ctrl_Tree_CompontentInstance, CEditorMainFrame::OnActivateComponentInstanceTreeItem)
EVT_CHECKBOX(Ctrl_CheckBox_ShowGuidId, CEditorMainFrame::OnCheckBoxGuidId)
EVT_TOGGLEBUTTON(wxID_ANY, CEditorMainFrame::OnAuiButton)
END_EVENT_TABLE()

CEditorMainFrame::CEditorMainFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, MAINFRAMENORMALSIZE, wxDEFAULT_FRAME_STYLE | wxMAXIMIZE)
    , m_pWEditAnimation(NULL)
    , m_pWEditLanguage(NULL)
    , m_bIsLanguageSwitched(false)
    , m_bSearchTextUpdate(false)
    , m_pWEditUI(NULL)
    , m_pWPerformance(NULL)
    , m_pSelectedComponent(NULL)
    , m_pToolBarPerform(NULL)
    , m_pToolBarView(NULL)
    , m_itemId(NULL)
    , m_pSceneTreeCtrl(NULL)
    , m_pObjectTreeCtrl(NULL)
    , m_pResourceTreeCtrl(NULL)
    , m_pComponentFileMenu(NULL)
    , m_bIsShowGuidId(false)
    , m_Timer(this, wxID_ANY)
    , m_nCurrentViewID(INVALID_DATA)
{
    SetIcon(wxICON(sample));
}

CEditorMainFrame::~CEditorMainFrame()
{
    m_Timer.Stop();
    BEATS_SAFE_DELETE(m_pComponentFileMenu);
    m_Manager.UnInit();
}

void CEditorMainFrame::InitMenu()
{
    m_pMenuBar              = new wxMenuBar;
    m_pFileMenu             = new wxMenu;
    m_pEditMenu             = new wxMenu;
    m_pAssetsMenu           = new wxMenu;
    m_pGameObjMenu          = new wxMenu;
    m_pConponentMenu        = new wxMenu;
    m_pWindowMenu           = new wxMenu;
    m_pHelpMenu             = new wxMenu;
    m_pHelpLanguageMenu     = new wxMenu;
    m_pComponentFileMenu    = new wxMenu;

    m_pComponentFileMenu->Append(eFLMS_AddFile, _T("添加文件"));
    m_pComponentFileMenu->Append(eFLMS_AddFileFolder, _T("添加文件夹"));
    m_pComponentFileMenu->Append(eFLMS_Delete, _T("删除文件"));
    m_pComponentFileMenu->Append(eFLMS_OpenFileDirectory, _T("打开所在的文件夹"));

    m_pMenuBar->Append(m_pFileMenu, wxT("&File"));
    m_pMenuBar->Append(m_pEditMenu, wxT("&Edit"));
    m_pMenuBar->Append(m_pAssetsMenu, wxT("&Assets"));
    m_pMenuBar->Append(m_pGameObjMenu, wxT("&GameObject"));
    m_pMenuBar->Append(m_pConponentMenu, wxT("&Conponent"));
    m_pMenuBar->Append(m_pWindowMenu, wxT("&Window"));
    m_pMenuBar->Append(m_pHelpMenu, wxT("&Help"));

    m_pFileMenu->Append(Menu_File_Open, wxT("Open"));
    m_pFileMenu->Append(Menu_File_Close, wxT("Close"));
    m_pFileMenu->Append(Menu_File_Save, wxT("&Save"));
    m_pFileMenu->Append(Menu_File_Export, wxT("Export"));
    m_pEditMenu->Append(Menu_Edit_Animation, wxT("Edit Animation"));
    m_pEditMenu->Append(Menu_Edit_UI, wxT("Edit UI"));
    m_pEditMenu->Append(Menu_Edit_Language, wxT("Edit Language"));
    m_pHelpMenu->Append(Menu_Help_Language, wxT("Language"), m_pHelpLanguageMenu);

    m_pHelpLanguageMenu->Append(Menu_Help_Language_Chinese, wxT("Chinese"));
    m_pHelpLanguageMenu->Append(Menu_Help_Language_English, wxT("English"));

    SetMenuBar(m_pMenuBar);
}

void CEditorMainFrame::InitCtrls()
{
    CreateAuiToolBar();
    CreateAuiNoteBook();
    CreateTreeCtrl();
    CreateTimeBar();
    CreatePropertyGrid();
    CreatSplitter();

    AddPageToBook();
    m_Manager.Update();
}

void CEditorMainFrame::CreateAuiToolBar()
{
    wxAuiToolBarItemArray prepend_items;
    wxAuiToolBarItemArray append_items;
    wxAuiToolBarItem item;
    item.SetKind(wxITEM_SEPARATOR);
    append_items.Add(item);
    item.SetKind(wxITEM_NORMAL);
    item.SetId(ID_CustomizeToolbar);
    item.SetLabel(_("Customize..."));
    append_items.Add(item);

    wxAuiToolBar* tb1 = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW);
    tb1->SetToolBitmapSize(wxSize(16,16));
    tb1->AddTool(ID_TB_Radio1, wxT("Radio 1"), wxArtProvider::GetBitmap(wxART_ADD_BOOKMARK), wxT("Radio 1"), wxITEM_RADIO);
    tb1->AddTool(ID_TB_Radio2, wxT("Radio 2"), wxArtProvider::GetBitmap(wxART_DEL_BOOKMARK), wxT("Radio 2"), wxITEM_RADIO);
    tb1->AddTool(ID_TB_Radio3, wxT("Radio 3"), wxArtProvider::GetBitmap(wxART_HELP_SIDE_PANEL), wxT("Radio 3"), wxITEM_RADIO);
    tb1->AddTool(ID_TB_Radio4, wxT("Radio 4"), wxArtProvider::GetBitmap(wxART_HELP_SETTINGS), wxT("Radio 4"), wxITEM_RADIO);
    tb1->SetCustomOverflowItems(prepend_items, append_items);
    tb1->Realize();

    wxAuiToolBar* tb2 = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_HORIZONTAL);
    tb2->SetToolBitmapSize(wxSize(16,16));
    tb2->AddTool(ID_TB_FlushProperty, wxT("Disabled"), wxArtProvider::GetBitmap(wxART_HELP_BOOK));
    tb2->SetCustomOverflowItems(prepend_items, append_items);
    tb2->Realize();

    m_pToolBarView = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_HORIZONTAL);
    m_pToolBarView->SetToolBitmapSize(wxSize(16,16));
    m_pToolBarView->AddTool(ID_ViewButton_Scene, wxT("Scene"), wxArtProvider::GetBitmap(wxART_GO_HOME), wxT("Scene"), wxITEM_RADIO);
    m_pToolBarView->AddTool(ID_ViewButton_UI, wxT("UI"), wxArtProvider::GetBitmap(wxART_TIP), wxT("UI"), wxITEM_RADIO);
    m_pToolBarView->AddTool(ID_ViewButton_Ani, wxT("Ani"), wxArtProvider::GetBitmap(wxART_HARDDISK), wxT("Ani"), wxITEM_RADIO);
    m_pToolBarView->SetCustomOverflowItems(prepend_items, append_items);
    m_pToolBarView->Realize();

    m_pToolBarPerform = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxAUI_TB_DEFAULT_STYLE |
        wxAUI_TB_OVERFLOW |
        wxAUI_TB_TEXT |
        wxAUI_TB_HORZ_TEXT);
    m_pToolBarPerform->AddTool(ID_TB_PerformBtn, wxT("Performance"), wxArtProvider::GetBitmap(wxART_GO_FORWARD), wxEmptyString, wxITEM_CHECK);

    m_Manager.AddPane(tb1,
        wxAuiPaneInfo().Name(wxT("tb1")).
        ToolbarPane().Top().Position(0).Dockable(false));
    m_Manager.AddPane(tb2,
        wxAuiPaneInfo().Name(wxT("tb2")).
        ToolbarPane().Top().Position(1).Dockable(false));
    m_Manager.AddPane(m_pToolBarView,
        wxAuiPaneInfo().Name(wxT("View")).
        ToolbarPane().Top().Movable(false).Fixed().Gripper(false));
    m_Manager.AddPane(m_pToolBarPerform,
        wxAuiPaneInfo().Name(wxT("tb3")).
        ToolbarPane().Top().Position(2).
        Dockable(false));
}

void CEditorMainFrame::CreateAuiNoteBook()
{
    wxSize client_size = GetClientSize();
    long lStyle = wxAUI_NB_TOP | wxAUI_NB_TAB_FIXED_WIDTH;

    client_size = wxSize(client_size.GetWidth() * 0.14, client_size.GetHeight() * 0.15);
    wxBoxSizer* pSizer = new wxBoxSizer(wxVERTICAL);
    m_pLeftPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, client_size);
    m_pLeftPanel->SetSizer(pSizer);
    m_pLeft = new wxAuiNotebook(m_pLeftPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, lStyle);
    pSizer->Add(m_pLeft, 1, wxGROW|wxALL, 0);
    m_pSearch = new wxSearchCtrl(m_pLeftPanel, Ctrl_Search_TreeSearch, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    pSizer->Add(m_pSearch, 0, wxGROW|wxALL, 0);
    m_pSearch->ShowCancelButton(true);

    pSizer = new wxBoxSizer(wxVERTICAL);
    m_pRightPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, client_size);
    m_pRightPanel->SetSizer(pSizer);
    m_pCheckBoxPropGrid = new wxCheckBox(m_pRightPanel, Ctrl_CheckBox_ShowGuidId, wxT("ShowGI"));
    pSizer->Add(m_pCheckBoxPropGrid, 0, wxGROW|wxALL, 0);
    m_pRight = new wxAuiNotebook(m_pRightPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, lStyle);
    pSizer->Add(m_pRight, 1, wxGROW|wxALL, 0);

    client_size = wxSize(client_size.GetWidth(), client_size.GetHeight() * 1.3);
    pSizer = new wxBoxSizer(wxVERTICAL);
    m_pBottomPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, client_size);
    m_pBottomPanel->SetSizer(pSizer);
    m_pBottom = new wxAuiNotebook(m_pBottomPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, lStyle);
    pSizer->Add(m_pBottom, 1, wxGROW|wxALL, 0);

    pSizer = new wxBoxSizer(wxVERTICAL);
    m_pCenterPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    m_pCenterPanel->SetSizer(pSizer);
    m_pCenter = new wxAuiNotebook(m_pCenterPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, lStyle);
    m_pSceneViewBtn = new wxToggleButton(m_pCenterPanel, ID_ViewButton_Scene, _T("Scene"));
    m_pUIViewBtn = new wxToggleButton(m_pCenterPanel, ID_ViewButton_UI, _T("UI"));
    m_pAniViewBtn = new wxToggleButton(m_pCenterPanel, ID_ViewButton_Ani, _T("Ani"));
    wxBoxSizer* pButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    pButtonSizer->Add(m_pSceneViewBtn, 0, wxALIGN_CENTER|wxALL, 0);
    pButtonSizer->Add(m_pUIViewBtn, 0, wxALIGN_CENTER|wxALL, 0);
    pButtonSizer->Add(m_pAniViewBtn, 0, wxALIGN_CENTER|wxALL, 0);
    pSizer->Add(pButtonSizer, 0, wxALIGN_CENTER|wxALL, 0);
    pSizer->Add(m_pCenter, 1, wxGROW|wxALL, 0);
}

void CEditorMainFrame::CreateTimeBar()
{
    m_pTimeBar = new CTimeBarFrame(m_pBottom);
}

void CEditorMainFrame::CreatePropertyGrid()
{
    int style = wxPG_BOLD_MODIFIED | wxPG_SPLITTER_AUTO_CENTER | wxPG_TOOLBAR | wxPG_DESCRIPTION | wxPG_TOOLTIPS;
    int extraStyle = wxPG_EX_MODE_BUTTONS | wxPG_EX_MULTIPLE_SELECTION;

    m_pPropGridManager = new CEnginePropertyGirdManager();
    m_pPropGridManager->Create(m_pRight, wxID_ANY, wxDefaultPosition, wxSize(100, 100), style );
    m_pPropGrid = m_pPropGridManager->GetGrid();
    m_pPropGridManager->SetExtraStyle(extraStyle);
    m_pPropGridManager->SetValidationFailureBehavior( wxPG_VFB_MARK_CELL | wxPG_VFB_SHOW_MESSAGEBOX );
    m_pPropGrid->SetVerticalSpacing( 2 );
    m_pPropGridManager->AddPage(wxT("page"));
}

void CEditorMainFrame::CreateTreeCtrl()
{
    wxImageList *pFileIconImages = new wxImageList(15, 15, true);
    wxIcon fileIcons[eTCIT_Count];
    wxSize iconSize(15, 15);
    fileIcons[eTCIT_File] = wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_LIST, iconSize);
    fileIcons[eTCIT_FileSelected] = wxArtProvider::GetIcon(wxART_FILE_OPEN, wxART_LIST, iconSize);
    fileIcons[eTCIT_Folder] = wxArtProvider::GetIcon(wxART_FOLDER, wxART_LIST, iconSize);
    fileIcons[eTCIT_FolderSelected] = wxArtProvider::GetIcon(wxART_FOLDER, wxART_LIST, iconSize);
    fileIcons[eTCIT_FolderOpened] = wxArtProvider::GetIcon(wxART_FOLDER_OPEN, wxART_LIST, iconSize);

    long iStyle = wxTR_DEFAULT_STYLE | wxTR_FULL_ROW_HIGHLIGHT | wxTR_NO_LINES | wxNO_BORDER | wxTR_EDIT_LABELS | wxTR_TWIST_BUTTONS;
    for (size_t i = 0; i < eTCIT_Count; ++i)
    {
        pFileIconImages->Add(fileIcons[i]);
    }
    m_pComponentFileTC = new wxTreeCtrl(m_pLeft, Ctrl_Tree_CompontentFile, wxPoint(0,0), wxSize(160,250), iStyle);
    m_pComponentFileTC->AssignImageList(pFileIconImages);
    m_pComponentFileTC->AddRoot(wxT("Component File"), eTCIT_Folder, -1, new CComponentFileTreeItemData(NULL, _T("")));

    pFileIconImages = new wxImageList(15, 15, true);
    for (size_t i = 0; i < eTCIT_Count; ++i)
    {
        pFileIconImages->Add(fileIcons[i]);
    }
    m_pComponentTC = new wxTreeCtrl(m_pLeft, Ctrl_Tree_Compontent, wxPoint(0,0), wxSize(160,250), iStyle);
    m_pComponentTC->AssignImageList(pFileIconImages);
    wxTreeItemId rootId = m_pComponentTC->AddRoot(wxT("Components"), eTCIT_Folder, -1, NULL);
    m_componentCatalogNameMap[_T("Root")] = rootId;
    m_componentTreeIdMap[0] = rootId;
    m_pComponentTC->Hide();

    pFileIconImages = new wxImageList(15, 15, true);
    for (size_t i = 0; i < eTCIT_Count; ++i)
    {
        pFileIconImages->Add(fileIcons[i]);
    }
    m_pComponentInstanceTreeCtrl = new wxTreeCtrl(m_pLeft, Ctrl_Tree_CompontentInstance, wxPoint(0,0), wxSize(160,250), iStyle);
    m_pComponentInstanceTreeCtrl->AssignImageList(pFileIconImages);
    m_pComponentInstanceTreeCtrl->AddRoot(wxT("ComponentInstance"), eTCIT_Folder, -1, NULL);
    m_pComponentInstanceTreeCtrl->Hide();
}

void CEditorMainFrame::AddPageToBook()
{
    m_pLeft->Freeze();
    m_pLeft->AddPage( m_pComponentFileTC, wxT("Component Model") );
    m_pLeft->Thaw();

    m_pRight->Freeze();
    m_pRight->AddPage( m_pPropGridManager, wxT("Inspector") );
    m_pRight->Thaw();

    m_pBottom->Freeze();
    m_pBottom->AddPage( m_pTimeBar, wxT("Animation") );
    m_pBottom->Thaw();

    m_pCenter->Freeze();
    m_pCenter->AddPage( m_pSplitter, wxT("wxTextCtrl 1") );
    m_pCenter->Thaw();

    m_Manager.AddPane(m_pLeftPanel, wxAuiPaneInfo().CenterPane().
        Name(wxT("Pane1")).
        Left());
    m_Manager.AddPane(m_pRightPanel, wxAuiPaneInfo().CenterPane().
        Name(wxT("Pane2")).
        Right());
    m_Manager.AddPane(m_pBottomPanel, wxAuiPaneInfo().CenterPane().Hide().
        Name(wxT("Pane3")).Caption(wxT("Pane Caption")).
        Bottom());
    m_Manager.AddPane(m_pCenterPanel, wxAuiPaneInfo().CenterPane().
        Name(wxT("Pane4")).Caption(wxT("Pane Caption")).
        Center());
}

void CEditorMainFrame::GetEditAnimationDialog()
{
    if (m_pWEditAnimation == NULL)
    {
        m_pWEditAnimation = new CEditAnimationDialog(this, wxID_ANY, wxT("Edit Animation"), wxDefaultPosition, MAINFRAMENORMALSIZE);
    }
    SetChildWindowStyle(m_pWEditAnimation);
    m_pWEditAnimation->ShowModal();
}

void CEditorMainFrame::GetEditLanguageDialog()
{
    if (m_pWEditLanguage == NULL)
    {
        m_pWEditLanguage = new CEditLanguageDialog(this, wxID_ANY, wxT("Edit Language"), wxPoint(500,200), wxSize(450,700));
    }
    if (m_bIsLanguageSwitched)
    {
        m_pWEditLanguage->LanguageSwitch();
    }
    m_pWEditLanguage->wxDialog::ShowModal();
}

void CEditorMainFrame::GetEditUIDialog()
{
    if (m_pWEditUI == NULL)
    {
        m_pWEditUI = new CEditUIDialog(this, wxID_ANY, wxT("Edit UI"), wxDefaultPosition, MAINFRAMENORMALSIZE);
    }
    SetChildWindowStyle(m_pWEditUI);
    m_pWEditUI->ShowModal();
}

void CEditorMainFrame::GetPerformanceDialog()
{
    if (m_pWPerformance == NULL)
    {
        m_pWPerformance = new EditPerformanceDialog(this, wxID_ANY, wxT("Performance"), wxPoint(500,200), wxSize(1000,700));
    }
    if (m_bIsLanguageSwitched)
    {
        m_pWPerformance->LanguageSwitch();
    }
    m_pWPerformance->wxDialog::Show();
}

void CEditorMainFrame::OnAuiButton(wxCommandEvent& event)
{
    int id = event.GetId();
    switch (id)
    {
    case Menu_File_Open:
        OpenProject();
        break;
    case Menu_File_Close:
        CloseProject();
        break;
    case Menu_File_Save:
        SaveProject();
        break;
    case Menu_File_Export:
        Export();
        break;
    case Menu_Edit_Animation:
        GetEditAnimationDialog();
        break;
    case Menu_Edit_UI:
        GetEditUIDialog();
        break;
    case Menu_Edit_Language:
        GetEditLanguageDialog();
        break;
    case ID_TB_PerformBtn:
        ShowPerformDlg();
        break;
    case Menu_Help_Language_Chinese:
        LanguageSwitch(Menu_Help_Language_Chinese);
        break;
    case Menu_Help_Language_English:
        LanguageSwitch(Menu_Help_Language_English);
        break;
    case eFLMS_AddFile:
        AddComponentFile();
        break;
    case eFLMS_AddFileFolder:
        AddComponentFileFolder();
        break;
    case eFLMS_Delete:
        DeleteComponentFile();
        break;
    case eFLMS_OpenFileDirectory:
        OpenComponentFileDirectory();
        break;
    case ID_TB_FlushProperty:
        SyncProperty();
        break;
    case ID_ViewButton_Scene:
        SelectViewID(ID_ViewButton_Scene);
        break;
    case ID_ViewButton_UI:
        SelectViewID(ID_ViewButton_UI);
        break;
    case ID_ViewButton_Ani:
        SelectViewID(ID_ViewButton_Ani);
        break;

    default:
        break;
    }
    return;
}

void CEditorMainFrame::CreatSplitter()
{
    m_pSplitter = new wxSplitterWindow(m_pCenter);
    m_pSplTop = new CFCEditorGLWindow(m_pSplitter);

    wxGLContext* pContext = static_cast<CEngineEditor*>(wxApp::GetInstance())->CreateGLContext(m_pSplTop);
    m_pSplTop->SetGLContext(pContext);
    m_pComponentRenderWindow = new CFCEditorComponentWindow(m_pSplitter, pContext);
    m_pComponentRenderWindow->SetMinSize(wxSize(MINWINDOWSIZE, MINWINDOWSIZE));
    m_pSplitter->SplitHorizontally(m_pSplTop, m_pComponentRenderWindow, 500);
    //TODO: Don't change the order, or the rendering is wrong. Don't know why
    RegisterUpdateWindow(m_pComponentRenderWindow);
    RegisterUpdateWindow(m_pSplTop);
}

void CEditorMainFrame::OnActivateComponentFile( wxTreeEvent& event )
{
    wxTreeItemId itemId = event.GetItem();
    m_itemTextcolor = m_pComponentFileTC->GetItemTextColour(itemId);
    m_pComponentFileTC->SetItemBackgroundColour(itemId, wxColor(150, 33, 184));
    if (m_itemId && m_itemId != itemId)
    {
        m_pComponentFileTC->SetItemBackgroundColour(m_itemId, m_itemTextcolor);
    }
    m_itemId = itemId;

    CComponentFileTreeItemData* pData = (CComponentFileTreeItemData*)m_pComponentFileTC->GetItemData(itemId);
    const TString& curWorkingFile = CComponentProxyManager::GetInstance()->GetCurrentWorkingFilePath();
    BEATS_ASSERT(pData != NULL)
    if (pData->IsDirectory())
    {
        m_pComponentFileTC->Toggle(itemId);
    }
    else if (_tcsicmp(pData->GetFileName().c_str(), curWorkingFile.c_str()) != 0)
    {
        if (curWorkingFile.length() > 0)
        {
            CloseComponentFile();
        }
        SelectComponent(NULL);
        const TCHAR* pszFileName = pData->GetFileName().c_str();
        OpenComponentFile(pszFileName);
        m_pComponentRenderWindow->UpdateAllDependencyLine();
    }
}

void CEditorMainFrame::OnComponentFileStartDrag( wxTreeEvent& event )
{
    CComponentFileTreeItemData* pData = static_cast<CComponentFileTreeItemData*>(m_pComponentFileTC->GetItemData(event.GetItem()));
    if (pData != NULL && !pData->IsDirectory())
    {
        m_pComponentFileTC->SelectItem(event.GetItem());
        m_pComponentRenderWindow->SetDraggingFileName(pData->GetFileName().c_str());
        event.Allow();
    }
}

void CEditorMainFrame::OnComponentFileEndDrag( wxTreeEvent& event )
{
    wxTreeItemId currentItemId = event.GetItem();
    if (currentItemId.IsOk())
    {
        CComponentFileTreeItemData* pCurrentItemData = static_cast<CComponentFileTreeItemData*>(m_pComponentFileTC->GetItemData(currentItemId));
        wxTreeItemId lastItemId = m_pComponentFileTC->GetSelection();
        CComponentFileTreeItemData* pLastItemData = static_cast<CComponentFileTreeItemData*>(m_pComponentFileTC->GetItemData(lastItemId));
        BEATS_ASSERT(pCurrentItemData != NULL && pLastItemData != NULL && !pLastItemData->IsDirectory());
        if (lastItemId != currentItemId)
        {
            CComponentFileTreeItemData* pNewData = new CComponentFileTreeItemData(pLastItemData->GetProjectDirectory(), pLastItemData->GetFileName());
            wxString lastItemText = m_pComponentFileTC->GetItemText(lastItemId);
            if (pCurrentItemData->IsDirectory())
            {
                m_pComponentFileTC->AppendItem(currentItemId, lastItemText, CEnginePropertyGirdManager::eTCIT_File, -1, pNewData);
            }
            else
            {
                wxTreeItemId parentId = m_pComponentFileTC->GetItemParent(currentItemId);
                m_pComponentFileTC->InsertItem(parentId, currentItemId, lastItemText, CEnginePropertyGirdManager::eTCIT_File, -1, pNewData);
            }
            m_pComponentFileTC->Expand(currentItemId);
            wxTreeItemId lastItemDirectory = m_pComponentFileTC->GetItemParent(lastItemId);
            m_pComponentFileTC->Delete(lastItemId);
            if (lastItemDirectory.IsOk())
            {
                if (m_pComponentFileTC->HasChildren(lastItemDirectory) == false)
                {
                    m_pComponentFileTC->SetItemImage(lastItemDirectory, CEnginePropertyGirdManager::eTCIT_Folder);
                }
            }
        }
    }
    if (m_pComponentRenderWindow->IsMouseInWindow())
    {
        const TCHAR* pDraggingFileName = m_pComponentRenderWindow->GetDraggingFileName();
        if (pDraggingFileName != NULL && PathFileExists(pDraggingFileName))
        {
            const TString& strCurWorkingFile = CComponentProxyManager::GetInstance()->GetCurrentWorkingFilePath();
            if (!strCurWorkingFile.empty())
            {
                int iRet = wxMessageBox(wxString::Format(_T("是否要将组件从\n%s\n拷贝到\n%s？"), pDraggingFileName, strCurWorkingFile.c_str()), _T("自动添加组件"), wxYES_NO);
                if (iRet == wxYES)
                {
                    CComponentProxyManager::GetInstance()->OpenFile(pDraggingFileName, true);
                    m_pComponentRenderWindow->UpdateAllDependencyLine();
                }
            }
            else
            {
                wxMessageBox(_T("请先打开一个文件!"));
            }
        }
        else
        {
            wxMessageBox(wxString::Format(_T("添加文件%s失败，可能文件不存在。"), pDraggingFileName));
        }
    }
    m_pComponentRenderWindow->SetDraggingFileName(NULL);
}

void CEditorMainFrame::OnComponentStartDrag( wxTreeEvent& event )
{
    m_pComponentTC->SelectItem(event.GetItem());
    CComponentTreeItemData* pData = static_cast<CComponentTreeItemData*>(m_pComponentTC->GetItemData(event.GetItem()));
    if (pData != NULL && !pData->IsDirectory())
    {
        m_pComponentRenderWindow->SetDraggingComponent(static_cast<CComponentEditorProxy*>(CComponentProxyManager::GetInstance()->GetComponentTemplate(pData->GetGUID())));
        event.Allow();
        BEATS_PRINT(_T("StartDrag at Pos: %d %d\n"), event.GetPoint().x, event.GetPoint().y);
    }
}

void CEditorMainFrame::OnComponentEndDrag( wxTreeEvent& event )
{
    if (m_pComponentRenderWindow->IsMouseInWindow())
    {
        CComponentEditorProxy* pDraggingComponent = m_pComponentRenderWindow->GetDraggingComponent();
        CComponentEditorProxy* pNewComponent = static_cast<CComponentEditorProxy*>(CComponentProxyManager::GetInstance()->CreateComponent(pDraggingComponent->GetGuid(), true));
        int x = 0;
        int y = 0;
        pDraggingComponent->GetGraphics()->GetPosition(&x, &y);
        pNewComponent->GetGraphics()->SetPosition(x, y);
        SelectComponent(pNewComponent);
        UpdateComponentInstanceTreeCtrl();
    }
    m_pComponentRenderWindow->SetDraggingComponent(NULL);
    event;
    BEATS_PRINT(_T("EndDrag at Pos: %d %d\n"), event.GetPoint().x, event.GetPoint().y);
}

void CEditorMainFrame::OnIdle( wxIdleEvent& /*event*/ )
{
    if (m_pPropGridManager->IsNeedUpdatePropertyGrid())
    {
        m_pPropGridManager->SetUpdateFlag(false);
        UpdatePropertyGrid();
    }
    SearchInCurrentTree();
}

void CEditorMainFrame::UpdatePropertyGrid()
{
    m_pPropGridManager->ClearPage(0);
    if (GetSelectedComponent() != NULL)
    {
        m_pPropGridManager->InsertComponentsInPropertyGrid(GetSelectedComponent());
    }
}

void CEditorMainFrame::SelectComponent(CComponentEditorProxy* pComponentInstance)
{
    m_pPropGridManager->ClearPage(0);
    m_pSelectedComponent = pComponentInstance;
    if (pComponentInstance != NULL)
    {
        // If we select an instance(not a template).
        if (pComponentInstance->GetId() != -1)
        {
            m_pComponentFileTC->Unselect();

            int x = 0;
            int y = 0;
            pComponentInstance->GetGraphics()->GetPosition(&x, &y);
            kmVec2 worldPos;
            m_pComponentRenderWindow->ConvertGridPosToWorldPos(x, y, &worldPos.x, &worldPos.y);

            CCamera* pComponentWndCamera = m_pComponentRenderWindow->GetCamera();
            kmVec3 viewPos;
            pComponentWndCamera->GetViewPos(viewPos);
            kmVec2 offset = pComponentWndCamera->GetCenterOffset();
            bool bOutOfHorizontal = worldPos.x < -viewPos.x - fabs(offset.x) || worldPos.x > -viewPos.x + fabs(offset.x);
            bool bOutOfVertical = worldPos.y < -viewPos.y - fabs(offset.y) || worldPos.y > -viewPos.y + fabs(offset.y);
            if (bOutOfHorizontal || bOutOfVertical)
            {
                pComponentWndCamera->SetViewPos(-worldPos.x, -worldPos.y, 0);
            }
        }
        m_pPropGridManager->InsertComponentsInPropertyGrid(pComponentInstance);
        m_pPropGridManager->ExpandAll(false);
    }
}

CComponentEditorProxy* CEditorMainFrame::GetSelectedComponent()
{
    return m_pSelectedComponent;
}

void CEditorMainFrame::OnSearch( wxCommandEvent& /*event*/ )
{
    SearchInCurrentTree();
}

void CEditorMainFrame::ResolveIdConflict(const std::map<size_t, std::vector<size_t>>& conflictIdMap)
{
    if (conflictIdMap.size() > 0)
    {
        TCHAR szConflictInfo[10240];
        _stprintf(szConflictInfo, _T("工程中有%d个ID发生冲突，点击\"是\"开始解决,点击\"否\"退出."), conflictIdMap.size());
        int iRet = MessageBox(NULL, szConflictInfo, _T("解决ID冲突"), MB_YESNO);
        if (iRet == IDYES)
        {
            CComponentProject* pProject = CComponentProxyManager::GetInstance()->GetProject();
            size_t lAnswer = 0;
            std::map<size_t, std::vector<size_t>>::const_iterator iter = conflictIdMap.begin();
            for (; iter != conflictIdMap.end(); ++iter)
            {
                _stprintf(szConflictInfo, _T("Id为%d的组件出现于文件:\n"), iter->first);
                for (size_t i = 0; i < iter->second.size(); ++i)
                {
                    TCHAR szFileName[1024];
                    _stprintf(szFileName, _T("%d.%s\n"), i, pProject->GetComponentFileName(iter->second[i]).c_str());
                    _tcscat(szConflictInfo, szFileName);
                }
                _tcscat(szConflictInfo, _T("需要保留ID的文件序号是(填-1表示全部分配新ID):"));
                lAnswer = wxGetNumberFromUser(szConflictInfo, _T("promote"), _T("请填入序号"), lAnswer, -1, iter->second.size() - 1);
                for (size_t i = 0; i < iter->second.size(); ++i)
                {
                    pProject->ResolveIdForFile(iter->second[i], iter->first, i == lAnswer);
                }
            }
        }
        else
        {
            _exit(0);
        }
    }
}

void CEditorMainFrame::OpenProjectFile( const TCHAR* pPath )
{
    if (pPath != NULL && _tcslen(pPath) > 0)
    {
        CComponentProject* pProject = CComponentProxyManager::GetInstance()->GetProject();
        std::map<size_t, std::vector<size_t>> conflictIdMap;
        CComponentProjectDirectory* pProjectData = pProject->LoadProject(pPath, conflictIdMap);
        bool bEmptyProject = pProjectData == NULL;
        if (!bEmptyProject)
        {
            CComponentFileTreeItemData* pData = (CComponentFileTreeItemData*)m_pComponentFileTC->GetItemData(m_pComponentFileTC->GetRootItem());
            if (pData)
            {
                BEATS_SAFE_DELETE(pData)
            }
            InitializeComponentFileTree(pProjectData, m_pComponentFileTC->GetRootItem());
            ResolveIdConflict(conflictIdMap);
        }
        m_pComponentFileTC->ExpandAll();
    }
}

void CEditorMainFrame::CloseProjectFile()
{
    SelectComponent(NULL);
    if (CComponentProxyManager::GetInstance()->GetCurrentWorkingFilePath().length() > 0)
    {
        CloseComponentFile();
    }
    CComponentProxyManager::GetInstance()->GetProject()->CloseProject();
    wxTreeItemId rootItem = m_pComponentFileTC->GetRootItem();
    m_pComponentFileTC->DeleteChildren(rootItem);
    CComponentFileTreeItemData* pData = (CComponentFileTreeItemData*)m_pComponentFileTC->GetItemData(rootItem);
    if (pData)
    {
        BEATS_SAFE_DELETE(pData)
        m_pComponentFileTC->SetItemData(rootItem, new CComponentFileTreeItemData(NULL, _T("")));
    }
    CComponentFileTreeItemData* pRootItemData = static_cast<CComponentFileTreeItemData*>(m_pComponentFileTC->GetItemData(rootItem));
    if (pRootItemData)
    {
        pRootItemData->SetData(NULL);
    }
    m_componentFileListMap.clear(); 
    m_pComponentFileTC->SetItemImage(rootItem, eTCIT_Folder);
}

void CEditorMainFrame::Export()
{
    SaveProject();
    TString szBinaryPath;
    CUtilityManager::GetInstance()->AcquireSingleFilePath(false, NULL, szBinaryPath, _T("选择要导出的文件"), BINARIZE_FILE_EXTENSION_FILTER, NULL);
    if (szBinaryPath.length() > 0)
    {
        std::vector<TString> filesToExport;
        for (std::map<TString, wxTreeItemId>::iterator iter = m_componentFileListMap.begin(); iter != m_componentFileListMap.end(); ++iter)
        {
            CComponentFileTreeItemData* pData = (CComponentFileTreeItemData*)m_pComponentFileTC->GetItemData(iter->second);
            BEATS_ASSERT(pData != NULL && !pData->IsDirectory());
            TString filePath(pData->GetFileName());
            filesToExport.push_back(filePath);
        }
        const TCHAR* pExtensionStr = PathFindExtension(szBinaryPath.c_str());
        if (_tcsicmp(pExtensionStr, BINARIZE_FILE_EXTENSION) != 0)
        {
            szBinaryPath.append(BINARIZE_FILE_EXTENSION);
        }
        CComponentProxyManager::GetInstance()->Export(filesToExport, szBinaryPath.c_str());
        //Export function will cause these operation: open->export->close->change file->open->...->restore open the origin file.
        //So we will update the dependency line as if we have just open a new file.
        m_pComponentRenderWindow->UpdateAllDependencyLine();
        wxMessageBox(wxT("导出完毕!"));
    }
}

void CEditorMainFrame::SaveProject()
{
    CComponentProject* pProject = CComponentProxyManager::GetInstance()->GetProject();
    if (pProject->GetRootDirectory() != NULL)
    {
        TString szSavePath = static_cast<CEngineEditor*>(wxApp::GetInstance())->GetWorkingPath();
        szSavePath.append(_T("\\")).append(EXPORT_STRUCTURE_DATA_PATCH_XMLFILENAME);
        CComponentProxyManager::GetInstance()->SaveTemplate(szSavePath.c_str());
        // Save Instance File
        CComponentProject* pProject = CComponentProxyManager::GetInstance()->GetProject();
        TString strProjectFullPath = pProject->GetProjectFilePath();
        if (strProjectFullPath.length() == 0)
        {
            CUtilityManager::GetInstance()->AcquireSingleFilePath(true, NULL, strProjectFullPath, _T("保存文件"), COMPONENT_PROJECT_EXTENSION, NULL);
        }
        else
        {
            strProjectFullPath.append(pProject->GetProjectFileName());
        }
        if (strProjectFullPath.length() > 0)
        {
            FILE* pProjectFile = _tfopen(strProjectFullPath.c_str(), _T("w+"));
            BEATS_ASSERT(pProjectFile != NULL)
                if (pProjectFile)
                {
                    fclose(pProjectFile);
                    CComponentProxyManager::GetInstance()->GetProject()->SaveProject();
                    const TString curWorkingFile = CComponentProxyManager::GetInstance()->GetCurrentWorkingFilePath();
                    if (curWorkingFile.length() > 0)
                    {
                        CComponentProxyManager::GetInstance()->SaveToFile(curWorkingFile.c_str());
                    }
                }
                UpdatePropertyGrid();
        }
    }
}

void CEditorMainFrame::OpenComponentFile( const TCHAR* pFilePath )
{
    BEATS_ASSERT(pFilePath != NULL && pFilePath[0] != 0, _T("Invalid file path"));
    CComponentProxyManager::GetInstance()->OpenFile(pFilePath);
    size_t uPageCount = m_pLeft->GetPageCount();
    int iComponentListPageIndex = -1;
    for (size_t i = 0; i < uPageCount; ++i)
    {
        if (m_pLeft->GetPage(i) == m_pComponentTC)
        {
            iComponentListPageIndex = i;
            break;
        }
    }
    if (iComponentListPageIndex == -1)
    {
        m_pLeft->AddPage(m_pComponentTC, CLanguageManager::GetInstance()->GetText(eL_ChooseComponent));
        m_pLeft->AddPage(m_pComponentInstanceTreeCtrl,CLanguageManager::GetInstance()->GetText(eL_ComponentInstance));
    }
    UpdateComponentInstanceTreeCtrl();
}

void CEditorMainFrame::CloseComponentFile(bool bRemindSave /*= true*/)
{
    CComponentProxyManager* pComponentProxyManager = CComponentProxyManager::GetInstance();
    const TString& strCurWorkingFile = pComponentProxyManager->GetCurrentWorkingFilePath();
    if (strCurWorkingFile.length() > 0)
    {
        if (bRemindSave)
        {
            int iRet = wxMessageBox(wxString::Format(_T("是否要保存当前文件?\n%s"), strCurWorkingFile.c_str()), _T("保存文件"), wxYES_NO);
            if (iRet == wxYES)
            {
                pComponentProxyManager->SaveToFile(strCurWorkingFile.c_str());
            }
        }
        pComponentProxyManager->CloseFile();
    }
    HideTreeCtrl(m_pComponentInstanceTreeCtrl);
    HideTreeCtrl(m_pComponentTC);
}

void CEditorMainFrame::OpenProject()
{
    CComponentProject* pProject = CComponentProxyManager::GetInstance()->GetProject();
    TString strProjectFullPath = pProject->GetProjectFilePath();

    if (strProjectFullPath.length() == 0)
    {
        TString strWorkingPath = static_cast<CEngineEditor*>(wxApp::GetInstance())->GetWorkingPath();
        CUtilityManager::GetInstance()->AcquireSingleFilePath(false, NULL, strProjectFullPath, _T("选择要读取的文件"), COMPONENT_PROJECT_EXTENSION, strWorkingPath.c_str());
        if (strProjectFullPath.length() > 0)
        {
            OpenProjectFile(strProjectFullPath.c_str());
        }
    }
    else
    {
        CloseProjectFile();
    }
}

void CEditorMainFrame::CloseProject()
{
    CloseProjectFile();
}

void CEditorMainFrame::ActivateFile(const TCHAR* pszFileName)
{
    std::map<TString, wxTreeItemId>::iterator iter = m_componentFileListMap.find(pszFileName);
    if (iter != m_componentFileListMap.end())
    {
        m_pComponentFileTC->SelectItem(iter->second);
        wxTreeEvent simulateEvent;
        simulateEvent.SetItem(iter->second);
        OnActivateComponentFile(simulateEvent);
    }
}

void CEditorMainFrame::OnRightClickComponentFileList( wxTreeEvent& event )
{
    m_pComponentFileTC->SelectItem(event.GetItem());
    if (CComponentProxyManager::GetInstance()->GetProject()->GetRootDirectory() != NULL)
    {
        CComponentFileTreeItemData* pData = (CComponentFileTreeItemData*)(m_pComponentFileTC->GetItemData(event.GetItem()));
        BEATS_ASSERT(pData);
        m_pComponentFileMenu->Enable(eFLMS_AddFile, pData->IsDirectory());
        m_pComponentFileMenu->Enable(eFLMS_AddFileFolder, pData->IsDirectory());
        m_pComponentFileMenu->Enable(eFLMS_OpenFileDirectory, !pData->IsDirectory());

        PopupMenu(m_pComponentFileMenu);
    }
    event.Skip(false);
}

void CEditorMainFrame::AddComponentFile()
{
    wxTreeItemId item = m_pComponentFileTC->GetSelection();
    if (item.IsOk())
    {
        TString result;
        CUtilityManager::GetInstance()->AcquireSingleFilePath(true, NULL, result, _T("添加文件"), COMPONENT_FILE_EXTENSION_FILTER, NULL);
        if (result.length() > 0)
        {
            std::map<TString, wxTreeItemId>::iterator iter = m_componentFileListMap.find(result);
            if (iter != m_componentFileListMap.end())
            {
                int iRet = wxMessageBox(_T("该文件已经存在于项目之中！不能重复添加！是否跳转到该项目？"), _T("文件已存在"), wxYES_NO);
                if (iRet == wxYES)
                {
                    m_pComponentFileTC->SelectItem(iter->second);
                }
            }
            else
            {
                if (!PathFileExists(result.c_str()))
                {
                    const TCHAR* pExtensionStr = PathFindExtension(result.c_str());
                    if (_tcsicmp(pExtensionStr, COMPONENT_FILE_EXTENSION) != 0)
                    {
                        result.append(COMPONENT_FILE_EXTENSION);
                    }

                    TiXmlDocument document;
                    TiXmlDeclaration* pDeclaration = new TiXmlDeclaration("1.0","","");
                    document.LinkEndChild(pDeclaration);
                    TiXmlElement* pRootElement = new TiXmlElement("Root");
                    document.LinkEndChild(pRootElement);
                    // TCHAR to char trick.
                    wxString pathTCHAR(result.c_str());
                    document.SaveFile(pathTCHAR);
                }
                CComponentFileTreeItemData* pData = new CComponentFileTreeItemData(NULL, result);
                wxString fileName = wxFileNameFromPath(result.c_str());
                wxTreeItemId newItemId = m_pComponentFileTC->AppendItem(item, fileName, CEnginePropertyGirdManager::eTCIT_File, -1, pData);
                m_pComponentFileTC->Expand(item);
                m_componentFileListMap[result] = newItemId;

                // When Add a new file in project, we must validate the id in this file. if id is conflicted, we have to resolve it.
                CComponentFileTreeItemData* pCurItemData = static_cast<CComponentFileTreeItemData*>(m_pComponentFileTC->GetItemData(item));
                BEATS_ASSERT(pCurItemData != NULL && pCurItemData->IsDirectory());
                std::map<size_t, std::vector<size_t>> conflictMap;
                pCurItemData->GetProjectDirectory()->AddFile(result, conflictMap);
                ResolveIdConflict(conflictMap);
                ActivateFile(result.c_str());
            }
        }
    }
}

void CEditorMainFrame::AddComponentFileFolder()
{
    wxTreeItemId item = m_pComponentFileTC->GetSelection();
    TString strNewName = wxGetTextFromUser(_T("文件夹名"), _T("添加文件夹"), _T("New File Folder Name"));
    if (strNewName.length() > 0)
    {
        CComponentFileTreeItemData* pCurItemData = static_cast<CComponentFileTreeItemData*>(m_pComponentFileTC->GetItemData(item));
        BEATS_ASSERT(pCurItemData != NULL && pCurItemData->IsDirectory());
        CComponentProjectDirectory* pNewProjectData = pCurItemData->GetProjectDirectory()->AddDirectory(strNewName);
        if (pNewProjectData != NULL)
        {
            CComponentFileTreeItemData* pData = new CComponentFileTreeItemData(pNewProjectData, strNewName);
            m_pComponentFileTC->AppendItem(item, strNewName, CEnginePropertyGirdManager::eTCIT_Folder, -1, pData);
            m_pComponentFileTC->Expand(item);
        }
        else
        {
            MessageBox(NULL, _T("The name has already exist!"), _T("Error"), MB_OK);
        }
    }
}

void CEditorMainFrame::DeleteComponentFile()
{
    wxTreeItemId item = m_pComponentFileTC->GetSelection();
    int iRet = wxMessageBox(_T("是否要删除物理文件？"), _T("删除文件"), wxYES_NO);
    DeleteItemInComponentFileList(item, iRet == wxYES);
    SelectComponent(NULL);
    wxTreeItemId parentItem = m_pComponentFileTC->GetItemParent(item);
    CComponentFileTreeItemData* pCurItemData = static_cast<CComponentFileTreeItemData*>(m_pComponentFileTC->GetItemData(item));
    CComponentFileTreeItemData* pParentItemData = NULL;
    CComponentProject* pProject = CComponentProxyManager::GetInstance()->GetProject();
    if (parentItem.IsOk())
    {
        pParentItemData = static_cast<CComponentFileTreeItemData*>(m_pComponentFileTC->GetItemData(parentItem));
    }
    bool bIsRootDirectory = pParentItemData == NULL;
    if (!bIsRootDirectory)
    {
        BEATS_ASSERT(item != m_pComponentFileTC->GetRootItem(), _T("Only root doesn't have a parent!"));
        if (pCurItemData->IsDirectory())
        {
            bool bRet = pParentItemData->GetProjectDirectory()->DeleteDirectory(pCurItemData->GetProjectDirectory());
            bRet;
            BEATS_ASSERT(bRet, _T("Delete directory %s failed!"), pCurItemData->GetFileName());
        }
        else
        {
            size_t uFileId = pProject->GetComponentFileId(pCurItemData->GetFileName());
            BEATS_ASSERT(uFileId != 0xFFFFFFFF, _T("Can't find file %s at project"), pCurItemData->GetProjectDirectory()->GetName().c_str());
            bool bRet = pParentItemData->GetProjectDirectory()->DeleteFile(uFileId);
            bRet;
            BEATS_ASSERT(bRet, _T("Delete file %s failed!"), pCurItemData->GetFileName().c_str());
        }
        m_pComponentFileTC->Delete(item);
    }
    else
    {
        BEATS_ASSERT(item == m_pComponentFileTC->GetRootItem(), _T("Only root doesn't have a parent!"));
        CComponentProxyManager::GetInstance()->CloseFile();
        pProject->GetRootDirectory()->DeleteAll(true);
        m_pComponentFileTC->DeleteChildren(item);// Never delete the root item.
    }
}

void CEditorMainFrame::OpenComponentFileDirectory()
{
    wxTreeItemId item = m_pComponentFileTC->GetSelection();
    TString path = CComponentProxyManager::GetInstance()->GetProject()->GetProjectFilePath();
    CComponentFileTreeItemData* pCurItemData = static_cast<CComponentFileTreeItemData*>(m_pComponentFileTC->GetItemData(item));
    if (m_pComponentFileTC->GetRootItem() != item)
    {
        BEATS_ASSERT(!pCurItemData->IsDirectory(), _T("eFLMS_OpenFileDirectory command can only use on file!"));
        if (!pCurItemData->IsDirectory())
        {
            path = pCurItemData->GetFileName();
            size_t pos = path.rfind(_T('\\'));
            path.resize(pos);
        }
    }

    TCHAR szCommand[MAX_PATH];
    GetWindowsDirectory(szCommand, MAX_PATH);
    _tcscat(szCommand, _T("\\explorer.exe "));
    _tcscat(szCommand, path.c_str());
    _tsystem(szCommand);
}

void CEditorMainFrame::DeleteItemInComponentFileList( wxTreeItemId itemId, bool bDeletePhysicalFile )
{
    BEATS_ASSERT(itemId.IsOk());
    CComponentFileTreeItemData* pData = (CComponentFileTreeItemData*)m_pComponentFileTC->GetItemData(itemId);
    BEATS_ASSERT(pData != NULL);
    if (pData->IsDirectory())
    {
        wxTreeItemIdValue idValue;
        wxTreeItemId childId = m_pComponentFileTC->GetFirstChild(itemId, idValue);
        while (childId.IsOk())
        {
            DeleteItemInComponentFileList(childId, bDeletePhysicalFile);
            childId = m_pComponentFileTC->GetNextChild(itemId, idValue);
        }
    }
    else
    {
        TString ItemNameStr(pData->GetFileName());
        std::map<TString, wxTreeItemId>::iterator iter = m_componentFileListMap.find(ItemNameStr);
        BEATS_ASSERT(iter != m_componentFileListMap.end());
        m_componentFileListMap.erase(iter);
        if (bDeletePhysicalFile)
        {
            ::DeleteFile(pData->GetFileName().c_str());
        }
    }
}

void CEditorMainFrame::OnTemplateComponentItemChanged( wxTreeEvent& event )
{
    wxTreeItemId id = m_pComponentTC->GetSelection();
    if (id.IsOk())
    {
        CComponentTreeItemData* pData = static_cast<CComponentTreeItemData*>(m_pComponentTC->GetItemData(id));
        if (pData != NULL && !pData->IsDirectory())
        {
            CComponentEditorProxy* pComponent = static_cast<CComponentEditorProxy*>(CComponentProxyManager::GetInstance()->GetComponentTemplate(pData->GetGUID()));
            SelectComponent(pComponent);
        }
    }
    event.Skip();
}

CComponentEditorProxy* CreateComponentProxy(CComponentGraphic* pGraphics, size_t guid, size_t parentGuid, TCHAR* pszClassName)
{
    return new CComponentEditorProxy(pGraphics, guid, parentGuid, pszClassName);
}

CComponentGraphic* CreateGraphic()
{
    return new CComponentGraphic_GL();
}

void CEditorMainFrame::InitComponentsPage()
{
    const TString& strWorkPath = CResourcePathManager::GetInstance()->GetResourcePath(CResourcePathManager::eRPT_Work);
    CComponentProxyManager::GetInstance()->DeserializeTemplateData(strWorkPath.c_str(), CreateComponentProxy, CreateGraphic);
    const std::map<size_t, CComponentBase*>* pComponentsMap = CComponentProxyManager::GetInstance()->GetComponentTemplateMap();
    for (std::map<size_t, CComponentBase*>::const_iterator componentIter = pComponentsMap->begin(); componentIter != pComponentsMap->end(); ++componentIter )
    {
        CComponentEditorProxy* pComponent = static_cast<CComponentEditorProxy*>(componentIter->second);
        const TString& catalogName = pComponent->GetCatalogName();
        std::map<TString, wxTreeItemId>::iterator iter = m_componentCatalogNameMap.find(catalogName);
        //Build catalog
        if (iter == m_componentCatalogNameMap.end())
        {
            std::vector<TString> result;
            CStringHelper::GetInstance()->SplitString(catalogName.c_str(), _T("\\"), result);
            BEATS_ASSERT(result.size() > 0);
            TString findStr;
            wxTreeItemId parentId = m_componentTreeIdMap[0];
            for (size_t i = 0; i < result.size(); ++i)
            {
                if (i > 0)
                {
                    findStr.append(_T("\\"));
                }
                findStr.append(result[i]);
                std::map<TString, wxTreeItemId>::iterator iter = m_componentCatalogNameMap.find(findStr);

                if (iter == m_componentCatalogNameMap.end())
                {
                    CComponentTreeItemData* pComponentItemData = new CComponentTreeItemData(true, 0);
                    parentId = m_pComponentTC->AppendItem(parentId, result[i], eTCIT_Folder, -1, pComponentItemData);
                    m_componentCatalogNameMap[findStr] = parentId;
                }
                else
                {
                    parentId = iter->second;
                }
            }
        }
        iter = m_componentCatalogNameMap.find(catalogName);
        BEATS_ASSERT(iter != m_componentCatalogNameMap.end());
        size_t guid = pComponent->GetGuid();
        CComponentTreeItemData* pComponentItemData = new CComponentTreeItemData(false, guid);
        m_componentTreeIdMap[guid] = m_pComponentTC->AppendItem(iter->second, pComponent->GetDisplayName(), eTCIT_File, -1, pComponentItemData);
    }
    m_pComponentTC->ExpandAll();
}

void CEditorMainFrame::InitializeComponentFileTree( CComponentProjectDirectory* pProjectData, const wxTreeItemId& id )
{
    CComponentFileTreeItemData* pData = new CComponentFileTreeItemData(pProjectData, pProjectData->GetName());
    m_pComponentFileTC->SetItemData(id, pData);
    const std::vector<CComponentProjectDirectory*>& children = pProjectData->GetChildren();
    for (std::vector<CComponentProjectDirectory*>::const_iterator iter = children.begin(); iter != children.end(); ++iter)
    {
        const TString& nameStr = (*iter)->GetName();
        wxTreeItemId newDirectoryId = m_pComponentFileTC->AppendItem(id, nameStr, eTCIT_Folder, -1, NULL);
        InitializeComponentFileTree(*iter, newDirectoryId);
    }

    const std::set<size_t>& files = pProjectData->GetFileList();
    for (std::set<size_t>::const_iterator iter = files.begin(); iter != files.end(); ++iter)
    {
        TString strComopnentFileName = CComponentProxyManager::GetInstance()->GetProject()->GetComponentFileName(*iter);
        CComponentFileTreeItemData* pData = new CComponentFileTreeItemData(NULL, strComopnentFileName);
        wxString pFileName = wxFileNameFromPath(strComopnentFileName);
        wxTreeItemId newFileId = m_pComponentFileTC->AppendItem(id, pFileName, eTCIT_File, -1, pData);
        BEATS_ASSERT(m_componentFileListMap.find(strComopnentFileName) == m_componentFileListMap.end(), _T("Same File in a project! %s"), strComopnentFileName.c_str());
        m_componentFileListMap[strComopnentFileName] = newFileId;
    }
}

void CEditorMainFrame::OnSplitterSashDClick( wxSplitterEvent& /*event*/ )
{
    int psn = 0;
    double gravity = 0.0;
    gravity = m_pSplitter->GetSashGravity();
    psn = m_pSplitter->GetSashPosition();
}

void CEditorMainFrame::InitFrame()
{
    m_Manager.SetManagedWindow(this);

    InitMenu();
    InitCtrls();

    SetStatusBar(new wxStatusBar(this));
    SetStatusText(CLanguageManager::GetInstance()->GetText(eL_Welcome));
    SetMinSize(wxSize(MINWINDOWSIZE,MINWINDOWSIZE));

    SelectViewID(ID_ViewButton_Scene);
    Centre();
}

void CEditorMainFrame::LanguageSwitch(int id)
{
    m_bIsLanguageSwitched = true;
    switch (id)
    {
    case Menu_Help_Language_Chinese:
        CLanguageManager::GetInstance()->LoadFromFile(eLT_Chinese);
        ChangeLanguage();
        break;
    case Menu_Help_Language_English:
        CLanguageManager::GetInstance()->LoadFromFile(eLT_English);
        ChangeLanguage();
        break;
    default:
        break;
    }
}

void CEditorMainFrame::ChangeLanguage()
{
    m_bIsLanguageSwitched = true;
    CLanguageManager* pLanguageManager = CLanguageManager::GetInstance();

    m_pMenuBar->SetMenuLabel(eMB_File, pLanguageManager->GetText(eL_File));
    m_pMenuBar->SetMenuLabel(eMB_Edit, pLanguageManager->GetText(eL_Edit));
    m_pMenuBar->SetMenuLabel(eMB_Assets, pLanguageManager->GetText(eL_Assets));
    m_pMenuBar->SetMenuLabel(eMB_GameObj, pLanguageManager->GetText(eL_GameObject));
    m_pMenuBar->SetMenuLabel(eMB_Conponent, pLanguageManager->GetText(eL_Conponent));
    m_pMenuBar->SetMenuLabel(eMB_Window, pLanguageManager->GetText(eL_Window));
    m_pMenuBar->SetMenuLabel(eMB_Help, pLanguageManager->GetText(eL_Help));

    m_pFileMenu->SetLabel(Menu_File_Open, pLanguageManager->GetText(eL_Open));
    m_pFileMenu->SetLabel(Menu_File_Close, pLanguageManager->GetText(eL_Close));
    m_pFileMenu->SetLabel(Menu_File_Save, pLanguageManager->GetText(eL_Save));
    m_pFileMenu->SetLabel(Menu_File_Export, pLanguageManager->GetText(eL_Export));

    m_pEditMenu->SetLabel(Menu_Edit_Animation, pLanguageManager->GetText(eL_Edit_Animation));
    m_pEditMenu->SetLabel(Menu_Edit_UI, pLanguageManager->GetText(eL_Edit_UI));
    m_pEditMenu->SetLabel(Menu_Edit_Language, pLanguageManager->GetText(eL_Edit_Language));

    m_pHelpMenu->SetLabel(Menu_Help_Language, pLanguageManager->GetText(eL_Language));
    m_pHelpMenu->SetLabel(Menu_Help_Language_Chinese, pLanguageManager->GetText(eL_Chinese));
    m_pHelpMenu->SetLabel(Menu_Help_Language_English, pLanguageManager->GetText(eL_English));

    m_pLeft->SetPageText(0, pLanguageManager->GetText(eL_ComponetModel));
    m_pLeft->SetPageText(1, pLanguageManager->GetText(eL_ChooseComponent));
    m_pLeft->SetPageText(2, pLanguageManager->GetText(eL_ComponentInstance));
    m_pCenter->SetPageText(0, pLanguageManager->GetText(eL_View));
    m_pCheckBoxPropGrid->SetLabel(pLanguageManager->GetText(eL_ShowGuidId));
    m_pRight->SetPageText(0, pLanguageManager->GetText(eL_Inspector));
    m_pBottom->SetPageText(0, pLanguageManager->GetText(eL_View));
    m_pBottom->SetPageText(1, pLanguageManager->GetText(eL_View));
    m_pToolBarPerform->SetToolLabel(ID_TB_PerformBtn, pLanguageManager->GetText(eL_Performance));
    m_pToolBarPerform->Refresh(false);

    if (m_pWPerformance != NULL)
    {
        m_pWPerformance->LanguageSwitch();
    }
    SetStatusText(pLanguageManager->GetText(eL_Welcome));
}

void CEditorMainFrame::OnSearchTextUpdate( wxCommandEvent& /*event*/ )
{
    m_bSearchTextUpdate = true;
    m_uLastSearchTextUpdateTime = GetTickCount();
}

EditPerformanceDialog* CEditorMainFrame::GetPerformanceDialogPtr()
{
    return m_pWPerformance;
}

void CEditorMainFrame::SyncProperty()
{
    if ( (::GetKeyState(VK_SHIFT) & 0x8000) > 0 )
    {
        int iRet = MessageBox(NULL, _T("确定要同步当前所有组件吗？"), _T("同步所有组件"), MB_YESNO);
        if (iRet == IDYES)
        {
            const std::map<size_t, std::map<size_t, CComponentBase*>*>* pInstanceMap = CComponentProxyManager::GetInstance()->GetComponentInstanceMap();
            for (std::map<size_t, std::map<size_t, CComponentBase*>*>::const_iterator iter = pInstanceMap->begin(); iter != pInstanceMap->end(); ++iter)
            {
                for (std::map<size_t, CComponentBase*>::const_iterator subIter = iter->second->begin(); subIter != iter->second->end(); ++subIter)
                {
                    CComponentEditorProxy* pProxyInstance = static_cast<CComponentEditorProxy*>(subIter->second);
                    BEATS_ASSERT(pProxyInstance != NULL);
                    // We need to update the host component after the proxy's dependencies have been resolved.
                    pProxyInstance->UpdateHostComponent();
                }
            }
            MessageBox(NULL, _T("同步完毕！"), _T("同步所有组件"), MB_OK);
        }
    }
    else
    {
        if (m_pSelectedComponent != NULL)
        {
            m_pSelectedComponent->UpdateHostComponent();
        }
        else
        {
            MessageBox(NULL, _T("请先选中一个组件才能同步数据！"), _T("未选中组件"), MB_OK);
        }
    }
}

void CEditorMainFrame::SearchInCurrentTree()
{
    wxTreeCtrl* pTreeCtrl = static_cast<wxTreeCtrl*>(m_pLeft->GetCurrentPage());
    BEATS_ASSERT(pTreeCtrl != nullptr)

    if (m_bSearchTextUpdate && GetTickCount() - m_uLastSearchTextUpdateTime > 700)
    {
        m_bSearchTextUpdate = false;
        m_componentTreeIdSearchMap.clear();
        m_componentFileListSearchMap.clear();
        wxString szText = m_pSearch->GetValue();
        pTreeCtrl->CollapseAll();
        if (pTreeCtrl == m_pComponentFileTC)
        {
            for (auto iter = m_componentFileListMap.begin(); iter != m_componentFileListMap.end(); ++iter)
            {
                wxString lableText = pTreeCtrl->GetItemText(iter->second);
                bool bMatch = lableText.Find(szText) != -1;
                // Because we can't hide tree control item, I set the text color to bg color to simulate hide.
                pTreeCtrl->SetItemTextColour(iter->second, bMatch ? 0 : 0xFFFFFFFF);
                if (bMatch)
                {
                    pTreeCtrl->EnsureVisible(iter->second);
                    m_componentFileListSearchMap[iter->first] = iter->second;
                }
            }
        }
        else
        {
            for (std::map<size_t, wxTreeItemId>::iterator iter = m_componentTreeIdMap.begin(); iter != m_componentTreeIdMap.end(); ++iter)
            {
                wxString lableText = pTreeCtrl->GetItemText(iter->second);
                bool bMatch = lableText.Find(szText) != -1;
                // Because we can't hide tree control item, I set the text color to bg color to simulate hide.
                pTreeCtrl->SetItemTextColour(iter->second, bMatch ? 0 : 0xFFFFFFFF);
                if (bMatch)
                {
                    pTreeCtrl->EnsureVisible(iter->second);
                    m_componentTreeIdSearchMap[iter->first] = iter->second;
                }
            }
        }
    }
}

void CEditorMainFrame::ShowPerformDlg()
{
    if (m_pWPerformance != NULL && m_pWPerformance->IsShown())
    {
        m_pWPerformance->Show(false);
    }
    else
    {
        GetPerformanceDialog();
    }
}

wxAuiToolBar* CEditorMainFrame::GetAuiToolBarPerformPtr()
{
    return m_pToolBarPerform;
}

void CEditorMainFrame::SetChildWindowStyle( CEditDialogBase* pDlg )
{
    if (m_bIsLanguageSwitched)
    {
        pDlg->LanguageSwitch();
    }
    if (IsMaximized())
    {
        pDlg->SetWindowStyle(wxDEFAULT_FRAME_STYLE | wxMAXIMIZE);
    }
    else
    {
        pDlg->SetWindowStyle(wxDEFAULT_FRAME_STYLE);
    }
}

void CEditorMainFrame::OnTreeCtrlExpanded( wxTreeEvent& event )
{
    if (event.GetId() == Ctrl_Tree_Compontent)
    {
        m_pComponentTC->SetItemImage(event.GetItem(), eTCIT_FileSelected);
    }
    else if (event.GetId() == Ctrl_Tree_CompontentFile)
    {
        m_pComponentFileTC->SetItemImage(event.GetItem(), eTCIT_FileSelected);
    }
    else if (event.GetId() == Ctrl_Tree_CompontentInstance)
    {
        m_pComponentInstanceTreeCtrl->SetItemImage(event.GetItem(), eTCIT_FileSelected);
    }
}

void CEditorMainFrame::OnTreeCtrlCollapsed( wxTreeEvent& event )
{
    if (event.GetId() == Ctrl_Tree_Compontent)
    {
        m_pComponentTC->SetItemImage(event.GetItem(), eTCIT_Folder);
    }
    else if (event.GetId() == Ctrl_Tree_CompontentFile)
    {
        m_pComponentFileTC->SetItemImage(event.GetItem(), eTCIT_Folder);
    }
    else if (event.GetId() == Ctrl_Tree_CompontentInstance)
    {
        m_pComponentInstanceTreeCtrl->SetItemImage(event.GetItem(), eTCIT_Folder);
    }
}

void CEditorMainFrame::UpdateComponentInstanceTreeCtrl()
{
    m_pComponentInstanceTreeCtrl->DeleteChildren(m_pComponentInstanceTreeCtrl->GetRootItem());
    const std::map<size_t, std::map<size_t, CComponentBase*>*>* pInstanceMap = CComponentProxyManager::GetInstance()->GetComponentInstanceMap();
    for (std::map<size_t, std::map<size_t, CComponentBase*>*>::const_iterator iter = pInstanceMap->begin(); iter != pInstanceMap->end(); ++iter)
    {
        TString strName = CComponentProxyManager::GetInstance()->QueryComponentName(iter->first);
        //Add a directory to tree
        wxTreeItemId itemID = m_pComponentInstanceTreeCtrl->AppendItem(m_pComponentInstanceTreeCtrl->GetRootItem(), strName, eTCIT_Folder);
        for (std::map<size_t, CComponentBase*>::const_iterator subIter = iter->second->begin(); subIter != iter->second->end(); ++subIter)
        {
            //Add a file node to the directory.
            wxString strIdLabel = wxString::Format(_T("%d"), subIter->first);
            CComponentInstanceTreeItemData* pComponentInstanceItemData = new CComponentInstanceTreeItemData(subIter->second);
            m_pComponentInstanceTreeCtrl->AppendItem(itemID, strIdLabel, eTCIT_File, -1, pComponentInstanceItemData);
        }
    }
    m_pComponentInstanceTreeCtrl->ExpandAll();
}

void CEditorMainFrame::OnActivateComponentInstanceTreeItem( wxTreeEvent& event )
{
    wxTreeItemId activeId = event.GetItem();
    CComponentInstanceTreeItemData* pItemData = (CComponentInstanceTreeItemData*)m_pComponentInstanceTreeCtrl->GetItemData(activeId);
    CComponentEditorProxy* pComponentProxy = dynamic_cast<CComponentEditorProxy*>(pItemData->GetComponentBase());
    BEATS_ASSERT(pComponentProxy != NULL);
    SelectComponent(pComponentProxy);
}

void CEditorMainFrame::OnCheckBoxGuidId( wxCommandEvent& event )
{
    m_bIsShowGuidId = event.IsChecked();
    UpdatePropertyGrid();
}

bool CEditorMainFrame::GetShowGuidId()
{
    return m_bIsShowGuidId;
}

void CEditorMainFrame::OnTimer( wxTimerEvent& event )
{
    static size_t uFrameCount = 0;
    static size_t uLastTimeTick = GetTickCount();

    if (GetTickCount() - uLastTimeTick >= 1000 / 30)
    {
        for (auto iter : m_NeedUpdateWindowVector)
        {
            iter->OnTimer(event);
        }
        uLastTimeTick = GetTickCount();
        TCHAR szBuffer[32];
        _stprintf(szBuffer, _T("%d\n"), uFrameCount);
        OutputDebugString(szBuffer);
        uFrameCount = 0;
    }
    else
    {
        uFrameCount++;
    }
    wxApp::GetInstance()->ProcessIdle();//don't change the position. It's in order to respond the ExitEvent
}

void CEditorMainFrame::StartRenderUpdate()
{
    m_Timer.Start(1);
}

void CEditorMainFrame::RegisterUpdateWindow( CFCEditorGLWindow* pWindow )
{
    m_NeedUpdateWindowVector.push_back(pWindow);
}

void CEditorMainFrame::HideTreeCtrl( wxTreeCtrl* pTreeCtrl )
{
    size_t uPageCount = m_pLeft->GetPageCount();
    for (size_t i = 0; i < uPageCount; ++i)
    {
        if (m_pLeft->GetPage(i) == pTreeCtrl)
        {
            pTreeCtrl->Hide();
            m_pLeft->RemovePage(i);
        }
    }
}

void CEditorMainFrame::SelectViewID( int nViewID )
{
    m_pSceneViewBtn->SetValue(ID_ViewButton_Scene == nViewID);
    m_pUIViewBtn->SetValue(ID_ViewButton_UI == nViewID);
    m_pAniViewBtn->SetValue(ID_ViewButton_Ani == nViewID);

    if (m_nCurrentViewID != nViewID)
    {
        m_nCurrentViewID = nViewID;
        m_pToolBarView->ToggleTool(nViewID, true);
        m_pToolBarView->Refresh(false);
        m_pToolBarView->Update();
    }
}

int CEditorMainFrame::GetSelectdViewID()
{
    return m_nCurrentViewID;
}
