#ifndef FCENGINEEDITOR_FCENGINEEDITOR_H__INCLUDE
#define FCENGINEEDITOR_FCENGINEEDITOR_H__INCLUDE

#include <wx/notebook.h>
#include <wx/treectrl.h>
#include <wx/aui/aui.h>
#include "EngineProperGridManager.h"
#include "GLCanvas.h"
#include "wx/splitter.h"

class wxTreeCtrl;
class wxSearchCtrl;
class wxSplitterWindow;
class wxToggleButton;
class CTimeBarFrame;
class CComponentGraphic;
class CFCEditorGLWindow;
class CFCEditorComponentWindow;
class CEditDialogBase;
class EditPerformanceDialog;

enum ECtrlID
{
    ID_CustomizeToolbar = wxID_HIGHEST+1,
    ID_TB_Radio1,
    ID_TB_Radio2,
    ID_TB_Radio3,
    ID_TB_Radio4,

    ID_ViewButton_Scene,
    ID_ViewButton_UI,
    ID_ViewButton_Ani,
    ID_TB_FlushProperty,
    ID_TB_PerformBtn,

    Menu_File_Open,
    Menu_File_Close,
    Menu_File_Save,
    Menu_File_Export,

    Menu_Edit_Animation,
    Menu_Edit_UI,
    Menu_Edit_Language,
    Menu_Help_Language,
    Menu_Help_Language_Chinese,
    Menu_Help_Language_English,

    eFLMS_AddFile,
    eFLMS_AddFileFolder,
    eFLMS_Delete,
    eFLMS_OpenFileDirectory,

    Ctrl_Tree_CompontentFile,
    Ctrl_Tree_Compontent,
    Ctrl_Tree_CompontentInstance,
    Ctrl_Search_TreeSearch,
    Ctrl_CheckBox_ShowGuidId,

};
class CEditorMainFrame : public wxFrame
{
public:
    CEditorMainFrame(const wxString& title);
    virtual ~CEditorMainFrame();
    void InitFrame();
    void InitMenu();
    void InitCtrls();
    void CreateAuiToolBar();
    void CreateAuiNoteBook();
    void CreateTreeCtrl();
    void CreateTimeBar();
    void CreatePropertyGrid();
    void CreatSplitter();
    void AddPageToBook();
    void UpdatePropertyGrid();
    void CloseProjectFile();
    void OpenProject();
    void CloseProject();
    void Export();
    void SaveProject();
    void InitComponentsPage();
    void GetEditAnimationDialog();
    void GetEditLanguageDialog();
    void GetEditUIDialog();
    void GetPerformanceDialog();
    void AddComponentFile();
    void AddComponentFileFolder();
    void OpenComponentFileDirectory();
    void SyncProperty();
    void HideTreeCtrl(wxTreeCtrl* pTreeCtrl);
    void SearchInCurrentTree();
    void DeleteComponentFile();
    void OpenProjectFile( const TCHAR* pPath );
    void ActivateFile(const TCHAR* pszFileName);
    void OpenComponentFile( const TCHAR* pFilePath );
    void CloseComponentFile(bool bRemindSave = true);
    void SelectComponent(CComponentEditorProxy* pComponentInstance);
    void InitializeComponentFileTree(CComponentProjectDirectory* pProjectData, const wxTreeItemId& id);
    void ResolveIdConflict(const std::map<size_t, std::vector<size_t>>& conflictIdMap);
    void DeleteItemInComponentFileList(wxTreeItemId itemId, bool bDeletePhysicalFile);
    CComponentEditorProxy* GetSelectedComponent();
    void LanguageSwitch(int id);
    void ChangeLanguage();
    EditPerformanceDialog* GetPerformanceDialogPtr();
    wxAuiToolBar* GetAuiToolBarPerformPtr();
    void ShowPerformDlg();
    void SetChildWindowStyle(CEditDialogBase* pDlg);
    bool GetShowGuidId();
    void StartRenderUpdate();
    void RegisterUpdateWindow(CFCEditorGLWindow* pWindow);
    void SelectViewID(int nViewID);
    int GetSelectdViewID();

protected:
    void OnTimer(wxTimerEvent& event);
    void OnActivateComponentFile(wxTreeEvent& event);
    void OnRightClickComponentFileList( wxTreeEvent& event );
    void OnComponentFileStartDrag( wxTreeEvent& event );
    void OnComponentFileEndDrag( wxTreeEvent& event );
    void OnComponentStartDrag( wxTreeEvent& event );
    void OnComponentEndDrag( wxTreeEvent& event );
    void OnIdle(wxIdleEvent& event);
    void OnSearch(wxCommandEvent& event);
    void OnSearchTextUpdate( wxCommandEvent& event );
    void OnAuiButton(wxCommandEvent& event);
    void OnTemplateComponentItemChanged(wxTreeEvent& event);
    void OnSplitterSashDClick(wxSplitterEvent& event);
    void OnTreeCtrlExpanded(wxTreeEvent& event);
    void OnTreeCtrlCollapsed(wxTreeEvent& event);
    void OnActivateComponentInstanceTreeItem(wxTreeEvent& event);
    void OnCheckBoxGuidId(wxCommandEvent& event);

public:
    void UpdateComponentInstanceTreeCtrl();
    
private:
    wxColor             m_itemTextcolor;
    wxTreeItemId        m_itemId;

    wxMenuBar*          m_pMenuBar;
    wxMenu*             m_pFileMenu;
    wxMenu*             m_pEditMenu;
    wxMenu*             m_pAssetsMenu;
    wxMenu*             m_pGameObjMenu;
    wxMenu*             m_pConponentMenu;
    wxMenu*             m_pWindowMenu;
    wxMenu*             m_pHelpMenu;
    wxMenu*             m_pHelpLanguageMenu;
    wxMenu*             m_pComponentFileMenu;

    bool                m_bIsShowGuidId;
    bool                m_bIsLanguageSwitched;
    bool                m_bSearchTextUpdate;
    wxTimer             m_Timer;
    size_t              m_uLastSearchTextUpdateTime;
    int                 m_nCurrentViewID;
    wxAuiManager        m_Manager;

    wxPanel*            m_pLeftPanel;
    wxAuiNotebook*      m_pLeft;
    wxPanel*            m_pRightPanel;
    wxAuiNotebook*      m_pRight;
    wxPanel*            m_pBottomPanel;
    wxAuiNotebook*      m_pBottom;
    wxPanel*            m_pCenterPanel;
    wxAuiNotebook*      m_pCenter;

    wxAuiToolBar*       m_pToolBarView;
    wxAuiToolBar*       m_pToolBarPerform;
    wxSearchCtrl*       m_pSearch;
    wxTreeCtrl*         m_pComponentFileTC;
    wxTreeCtrl*         m_pComponentTC;
    wxTreeCtrl*         m_pComponentInstanceTreeCtrl;
    wxTreeCtrl*         m_pSceneTreeCtrl;
    wxTreeCtrl*         m_pObjectTreeCtrl;
    wxTreeCtrl*         m_pResourceTreeCtrl;
    CTimeBarFrame*      m_pTimeBar;
    wxPropertyGrid*     m_pPropGrid;
    wxCheckBox*         m_pCheckBoxPropGrid;
    wxToggleButton*     m_pSceneViewBtn;
    wxToggleButton*     m_pUIViewBtn;
    wxToggleButton*     m_pAniViewBtn;
    CEditDialogBase*            m_pWEditAnimation;
    CEditDialogBase*            m_pWEditLanguage;
    CEditDialogBase*            m_pWEditUI;
    EditPerformanceDialog*      m_pWPerformance;
    wxSplitterWindow*           m_pSplitter;
    CComponentEditorProxy*      m_pSelectedComponent;
    CFCEditorGLWindow*          m_pSplTop;
    CFCEditorComponentWindow*   m_pComponentRenderWindow;
    CEnginePropertyGirdManager*     m_pPropGridManager;
    std::vector<CFCEditorGLWindow*> m_NeedUpdateWindowVector;
    std::map<size_t, wxTreeItemId>  m_componentTreeIdMap; //use this map to quick find the wxItemId for a specific id of component
    std::map<size_t, wxTreeItemId>  m_componentTreeIdSearchMap;
    std::map<TString, wxTreeItemId> m_componentCatalogNameMap; //use this map to quick find the wxItemId for a catalog
    std::map<TString, wxTreeItemId> m_componentFileListMap; //use this map to quick find the wxItemId for a component file name.
    std::map<TString, wxTreeItemId> m_componentFileListSearchMap;

DECLARE_EVENT_TABLE();
};

#endif