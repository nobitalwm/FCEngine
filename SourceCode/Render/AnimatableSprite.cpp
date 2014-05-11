#include "stdafx.h"
#include "AnimatableSprite.h"
#include "Material.h"
#include "TextureFrag.h"
#include "QuadBatchRenderGroup.h"
#include "RenderGroupManager.h"

CAnimatableSprite::CAnimatableSprite(CTextureFrag *textureFrag)
    : CSprite(textureFrag)
    , m_uCurrFrame(0)
{
    InitAnimProp();
}

CAnimatableSprite::CAnimatableSprite(const TString &textureAtlasName, const TString &textureFragName)
    : CSprite(textureAtlasName, textureFragName)
    , m_uCurrFrame(0)
{
    InitAnimProp();
}

CAnimatableSprite::CAnimatableSprite(const TString &textureAtlasName, const TString &textureFragName, 
                                     const kmVec2 &size, const kmVec2 &origin)
    : CSprite(textureAtlasName, textureFragName, size, origin)
    , m_uCurrFrame(0)
{
    InitAnimProp();
}

CAnimatableSprite::CAnimatableSprite(const TString &textureAtlasName, const TString &textureFragName, 
                                     const kmVec2 &size)
    : CSprite(textureAtlasName, textureFragName, size)
    , m_uCurrFrame(0)
{
    InitAnimProp();
}

CAnimatableSprite::~CAnimatableSprite()
{
}

void CAnimatableSprite::InitAnimProp()
{
    AddProp(eAP_PROP_CURR_FRAME, _T("Current Frame"), 
        [this](kmScalar currFrame){SetCurrFrame((size_t)currFrame);},
        [this](){return (kmScalar)CurrFrame();});
}

void CAnimatableSprite::SetFrames(const TFrameList &frames)
{
    m_frames.assign(frames.begin(), frames.end());
}

void CAnimatableSprite::AddFrame(CTextureFrag *frame)
{
    m_frames.push_back(frame);
}

const CAnimatableSprite::TFrameList &CAnimatableSprite::Frames() const
{
    return m_frames;
}

void CAnimatableSprite::SetCurrFrame(size_t currFrame)
{
    if(currFrame >= m_frames.size())
    {
        m_uCurrFrame = m_frames.size() - 1;
    }
    else
    {
        m_uCurrFrame = currFrame;
    }
}

size_t CAnimatableSprite::CurrFrame() const
{
    return m_uCurrFrame;
}

void CAnimatableSprite::Render()
{
    if(m_frames.empty())
    {
        CSprite::Render();
    }
    else
    {
        BEATS_ASSERT(m_uCurrFrame < m_frames.size());
        Material()->SetTexture(0, m_frames[m_uCurrFrame]->Texture());

        kmMat4 transform;
        kmMat4Identity(&transform);

        CQuadBatchRenderGroup *renderGroup = dynamic_cast<CQuadBatchRenderGroup *>(
            CRenderGroupManager::GetInstance()->GetRenderGroup(CRenderGroupManager::LAYER_2D));
        BEATS_ASSERT(renderGroup);
        renderGroup->AddQuad(m_quad, m_frames[m_uCurrFrame]->Quad(), transform, Material());
    }
}
