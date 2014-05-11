#include "stdafx.h"
#include "Animation_ui.h"
#include "AnimationLayer.h"

using namespace FCGUI;

CAnimation::CAnimation(const TString &name)
    : m_replayMode(EReplayMode::eRM_ONCE)
    , m_strName(name)
    , m_fDuration(0.f)
{

}

CAnimation::~CAnimation()
{
    for(auto layer : m_layers)
    {
        BEATS_SAFE_DELETE(layer);
    }
}

const TString& CAnimation::Name() const
{
    return m_strName;
}

void CAnimation::SetDuration(kmScalar duration)
{
    m_fDuration = duration;
}

kmScalar CAnimation::Duration() const
{
    return m_fDuration;
}

void CAnimation::SetMode(EReplayMode replayMode)
{
    m_replayMode = replayMode;
}

CAnimation::EReplayMode CAnimation::Mode() const
{
    return m_replayMode;
}

CAnimationLayer *CAnimation::CreateLayer(const TString &name)
{
    CAnimationLayer *layer = new CAnimationLayer(name);
    layer->SetAnimation(this);
    m_layers.push_back(layer);
    return layer;
}

void CAnimation::DeleteLayer(CAnimationLayer *layer)
{
    for(size_t i = 0; i < m_layers.size(); ++i)
    {
        if(m_layers[i] == layer)
        {
            m_layers.erase(m_layers.begin() + i);
            BEATS_SAFE_DELETE(layer);
            break;
        }
    }
}

void CAnimation::Apply( CAnimatable *target, kmScalar position )
{
    if(position < 0.f)
        position = 0.f;
    if(position > m_fDuration)
        position = m_fDuration;

    for(auto layer : m_layers)
    {
        layer->Apply(target, position);
    }
}

