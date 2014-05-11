#include "stdafx.h"
#include "RenderLayer.h"
#include "Render/TextureFrag.h"
#include "Render/TextureFragManager.h"
#include "Render/Material.h"
#include "GUI/System.h"

using namespace FCGUI;

CRenderLayer::CRenderLayer(const TString &textureAtlasName, const TString &textureFragName )
{
    m_frames.push_back(CTextureFragManager::GetInstance()->GetTextureFrag(
        textureAtlasName, textureFragName));
    Init();
}

CRenderLayer::CRenderLayer( CTextureFrag *textureFrag )
{
    m_frames.push_back(textureFrag);
    Init();
}

CRenderLayer::CRenderLayer( const TFrameList &frames )
{
    m_frames.assign(frames.begin(), frames.end());
    Init();
}

CRenderLayer::~CRenderLayer()
{
}

void CRenderLayer::Init()
{
    m_uCurrFrame = 0;
    InitAnimProp();
    m_pMaterial = CSystem::GetInstance()->CreateDefaultMaterial();
    BEATS_ASSERT(m_uCurrFrame < m_frames.size());
    m_pMaterial->SetTexture(0, m_frames[m_uCurrFrame]->Texture());
}

void CRenderLayer::InitAnimProp()
{
    AddProp(PROP_CURR_FRAME, _T("Current Frame"), 
        [this](kmScalar currFrame){SetCurrFrame((size_t)currFrame);},
        [this](){return (kmScalar)CurrFrame();});
}

void CRenderLayer::SetCurrFrame(size_t currFrame)
{
    if(currFrame >= m_frames.size())
    {
        m_uCurrFrame = m_frames.size() - 1;
    }
    else
    {
        m_uCurrFrame = currFrame;
    }
    m_pMaterial->SetTexture(0, m_frames[m_uCurrFrame]->Texture());
}

size_t CRenderLayer::CurrFrame() const
{
    return m_uCurrFrame;
}

CTextureFrag *CRenderLayer::GetTextureFrag() const
{
    CTextureFrag *frag = nullptr;
    if(m_uCurrFrame < m_frames.size())
    {
        frag = m_frames[m_uCurrFrame];
    }
    return frag;
}

SharePtr<CMaterial> CRenderLayer::GetMaterial() const
{
    return m_pMaterial;
}