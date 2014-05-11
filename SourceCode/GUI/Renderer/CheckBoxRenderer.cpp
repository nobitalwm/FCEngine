#include "stdafx.h"
#include "CheckBoxRenderer.h"
#include "Render/TextureFragManager.h"
using namespace FCGUI;

CCheckBoxRenderer::CCheckBoxRenderer()
{
 
}

CCheckBoxRenderer::~CCheckBoxRenderer()
{

}

void CCheckBoxRenderer::AddLayer(const TString &contentTextureName, const TString &indicatorTextureName, CCheckBox::EStateType state)
{
     CCheckBox *check = static_cast<CCheckBox *>(m_pWindow); 
    
     CBaseRenderer::AddLayer(indicatorTextureName,state);

     CWindow *sub = check->GetSubWindow();
     CBaseRenderer* checkrender = sub->Renderer();
     unsigned int contentState = CWindow::eST_STATE_NORMAL;
     if( state == CCheckBox::eST_STATE_DISABLED_CHECKED || state == CCheckBox::eST_STATE_DISABLED_UNCHECKED )
     {
         contentState = CWindow::eST_STATE_DISABLED;
     }
     checkrender->AddLayer(contentTextureName,contentState);
}

void CCheckBoxRenderer::AddLayer(CTextureFrag *contentLayer, CTextureFrag* indicatorLayer, CCheckBox::EStateType state)
{
    CCheckBox *check = static_cast<CCheckBox *>(m_pWindow);
    
    CBaseRenderer::AddLayer(indicatorLayer,state);

    CWindow *sub = check->GetSubWindow();
    CBaseRenderer* checkrender =sub->Renderer();
    unsigned int contentState = CWindow::eST_STATE_NORMAL;
    if( state == CCheckBox::eST_STATE_DISABLED_CHECKED || state == CCheckBox::eST_STATE_DISABLED_UNCHECKED )
    {
        contentState = CWindow::eST_STATE_DISABLED;
    }
    checkrender->AddLayer(contentLayer,contentState);
}


