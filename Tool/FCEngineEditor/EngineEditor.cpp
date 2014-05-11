#include "stdafx.h"
#include "EngineEditor.h"
#include "EditorMainFrame.h"
#include "EngineSplash.h"
#include "wxPtrButtonEditor.h"
#include "EngineCenter.h"
#include "ComponentGraphics_GL.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentProxyManager.h"
#include "Vec2fPropertyDescription.h"
#include "Vec3fPropertyDescription.h"
#include "Vec4fPropertyDescription.h"

CEngineEditor::CEngineEditor()
    : m_glRC(NULL)
    , m_pMainFrame(NULL)
    , m_pPtrButtonEditor(NULL)
{

}

CEngineEditor::~CEngineEditor()
{
    BEATS_SAFE_DELETE(m_glRC);
}

bool CEngineEditor::OnInit()
{
    bool ret = wxApp::OnInit();
    if ( ret )
    {
        CLanguageManager::GetInstance()->LoadFromFile(eLT_English);
        m_pMainFrame = new CEditorMainFrame(wxT("FCEngineEditor"));
        m_pMainFrame->InitFrame();
        m_pMainFrame->ChangeLanguage();
        m_pMainFrame->Show(true);
        CEngineCenter::GetInstance()->Initialize();
        CreatePtrEditor();
        m_pMainFrame->InitComponentsPage();
#ifndef _DEBUG
        wxBitmap bmp;
        bmp.LoadFile(wxT("../resource/splash.png"), wxBITMAP_TYPE_PNG);
        CEngineSplash* pSplash = new CEngineSplash(&bmp,
            wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT,
            0, m_pMainFrame, wxID_ANY, wxDefaultPosition, wxDefaultSize,
            wxSIMPLE_BORDER|wxSTAY_ON_TOP);
        pSplash->Instance();
        BEATS_SAFE_DELETE(pSplash);
#endif
        // Set working path.
        TCHAR szPath[MAX_PATH];
        GetModuleFileName(NULL, szPath, MAX_PATH);
        int iStrPos = CStringHelper::GetInstance()->FindLastString(szPath, _T("\\"), false);
        BEATS_ASSERT(iStrPos >= 0);
        szPath[iStrPos] = 0;
        m_strWorkingPath.assign(szPath);
        m_pMainFrame->StartRenderUpdate();
    }
    return ret;
}
int CEngineEditor::OnExit()
{
    wxApp::OnExit();
    CVec2PropertyDescription::m_pXBasicPropertyInfo.Destroy();
    CVec2PropertyDescription::m_pYBasicPropertyInfo.Destroy();
    CVec3PropertyDescription::m_pZBasicPropertyInfo.Destroy();
    CVec4PropertyDescription::m_pWBasicPropertyInfo.Destroy();
    CComponentProxyManager::Destroy();
    CEngineCenter::Destroy();
    return 0;
}

wxPGEditor* CEngineEditor::GetPtrEditor() const
{
    return m_pPtrButtonEditor;
}

void CEngineEditor::CreatePtrEditor()
{
    m_pPtrButtonEditor = wxPropertyGrid::RegisterEditorClass(new wxPtrButtonEditor());;
}

CEditorMainFrame* CEngineEditor::GetMainFrame() const
{
    return m_pMainFrame;
}

wxGLContext* CEngineEditor::CreateGLContext(wxGLCanvas* pCanvas)
{
    m_glRC = new wxGLContext(pCanvas);
    return m_glRC;
}

wxGLContext* CEngineEditor::GetGLContext() const
{
    return m_glRC;
}

const TString& CEngineEditor::GetWorkingPath()
{
    return m_strWorkingPath;
}

IMPLEMENT_APP(CEngineEditor)