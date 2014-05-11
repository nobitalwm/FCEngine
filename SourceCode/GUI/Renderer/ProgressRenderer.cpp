#include "stdafx.h"
#include "ProgressRenderer.h"
#include "Render/TextureFragManager.h"
using namespace FCGUI;

CProgressRenderer::CProgressRenderer()
{

}

CProgressRenderer::~CProgressRenderer()
{

}

void  CProgressRenderer::AddLayer(const TString &fillTextName, const TString &backTextName, CProgress::EStateType state)
{
    CProgress *process = static_cast<CProgress *>(m_pWindow); 
    CBaseRenderer::AddLayer(backTextName,state);

    CWindow *sub = process->GetSubWndFill();
    CBaseRenderer* subrender = sub->Renderer();
    subrender->AddLayer(fillTextName);
}

void  CProgressRenderer::AddLayer(CTextureFrag* fillLayer, CTextureFrag *backLayer, CProgress::EStateType state)
{
    CProgress *process = static_cast<CProgress *>(m_pWindow); 
    CBaseRenderer::AddLayer(backLayer,state);

    CWindow *sub = process->GetSubWndFill();
    CBaseRenderer* subrender = sub->Renderer();
    subrender->AddLayer(fillLayer);
}
