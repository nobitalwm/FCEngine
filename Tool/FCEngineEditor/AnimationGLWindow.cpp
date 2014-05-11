#include "stdafx.h"
#include "AnimationGLWindow.h"
#include "Render/Model.h"
#include "Utility/BeatsUtility/SharePtr.h"
#include "Render/AnimationController.h"
#include "EditAnimationDialog.h"
#include "timebarframe.h"

BEGIN_EVENT_TABLE(CAnimationGLWindow, CFCEditorGLWindow)
    EVT_TIMER(wxID_ANY, CAnimationGLWindow::OnTimer)
END_EVENT_TABLE()

CAnimationGLWindow::CAnimationGLWindow(wxWindow *parent, wxGLContext* pContext, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
    : CFCEditorGLWindow(parent, pContext, id, pos, size, style, name)
    , m_pModel(NULL)
{

}

CAnimationGLWindow::~CAnimationGLWindow()
{
}

void CAnimationGLWindow::OnTimer(wxTimerEvent& event)
{
    if (IsShownOnScreen())
    {
        CEditAnimationDialog* pDialog = (CEditAnimationDialog*)GetParent();
        if (m_pModel && m_pModel->GetAnimationController()->IsPlaying())
        {
            int iCur = m_pModel->GetAnimationController()->GetCurrFrame();
            pDialog->GetTimeBar()->SetCurrentCursor(iCur);
        }
        super::OnTimer(event);
    }
}

void CAnimationGLWindow::SetModel( CModel* pModel )
{
    m_pModel = pModel;
}

CModel* CAnimationGLWindow::GetModel()
{
    return m_pModel;
}
