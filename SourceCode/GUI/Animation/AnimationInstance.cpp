#include "stdafx.h"
#include "AnimationInstance.h"
#include "AnimationManager_ui.h"
#include "Event/BaseEvent.h"

using namespace FCGUI;

CAnimationInstance::CAnimationInstance(CAnimation *animation, CAnimatable *target, 
                                     CAnimation::EReplayMode replayMode, 
                                     kmScalar speed, bool destroyOnStop)
    : m_pAnimation(animation)
    , m_pTarget(target)
    , m_replayMode(replayMode == CAnimation::EReplayMode::eRM_DEFAULT ? m_pAnimation->Mode() : replayMode)
    , m_fSpeed(speed)
    , m_fPosition(0.f)
    , m_bBackward(false)
    , m_bPlaying(false)
    , m_bDestroyOnStop(destroyOnStop)
    , m_bDestroyed(false)
{
    Stop();
}

CAnimationInstance::~CAnimationInstance()
{

}

void CAnimationInstance::Update( kmScalar dt )
{
    if(m_bPlaying)
    {
        dt *= m_fSpeed;
        if(isReverse() != m_bBackward)
            dt = -dt;
        kmScalar newposition = m_fPosition + dt;
        if(newposition > m_pAnimation->Duration())
        {
            if(isLoop())
            {
                m_fPosition = newposition - m_pAnimation->Duration();
                CBaseEvent event(eET_EVENT_LOOPED);
                DispatchEvent(&event);
            }
            else if(isBounce())
            {
                m_fPosition = m_pAnimation->Duration() - (newposition - m_pAnimation->Duration());
                m_bBackward = !m_bBackward;
                CBaseEvent event(eET_EVENT_BOUNCED);
                DispatchEvent(&event);
            }
            else
            {
                m_fPosition = m_pAnimation->Duration();
                if(m_bDestroyOnStop)
                    m_bDestroyed = true;
                CBaseEvent event(eET_EVENT_STOPED);
                DispatchEvent(&event);
            }
        }
        else if(newposition < 0.f)
        {
            if(isLoop())
            {
                m_fPosition = m_pAnimation->Duration() + newposition;
                CBaseEvent event(eET_EVENT_LOOPED);
                DispatchEvent(&event);
            }
            else if(isBounce())
            {
                m_fPosition = - newposition;
                m_bBackward = !m_bBackward;
                CBaseEvent event(eET_EVENT_BOUNCED);
                DispatchEvent(&event);
            }
            else
            {
                m_fPosition = 0.f;
                if(m_bDestroyOnStop)
                    m_bDestroyed = true;
                CBaseEvent event(eET_EVENT_STOPED);
                DispatchEvent(&event);
            }
        }
        else
        {
            m_fPosition = newposition;
        }

        m_pAnimation->Apply(m_pTarget, m_fPosition);
    }
}

void CAnimationInstance::Play()
{
    Stop();
    Resume();
    CBaseEvent event(eET_EVENT_STARTED);
    DispatchEvent(&event);
}

void CAnimationInstance::Pause()
{
    m_bPlaying = false;
    CBaseEvent event(eET_EVENT_PAUSED);
    DispatchEvent(&event);
}

void CAnimationInstance::Resume()
{
    m_bPlaying = true;
    CBaseEvent event(eET_EVENT_RESUMED);
    DispatchEvent(&event);
}

void CAnimationInstance::Stop()
{
    m_bPlaying = false;
    m_fPosition = isReverse() ? m_pAnimation->Duration() : 0.f;
    CBaseEvent event(eET_EVENT_STOPED);
    DispatchEvent(&event);
}

void CAnimationInstance::Goto(kmScalar position)
{
    m_fPosition = fmodf(position, m_pAnimation->Duration());
}

void CAnimationInstance::GotoAndPause(kmScalar position)
{
    Goto(position);
    Pause();
}

void CAnimationInstance::GotoAndPlay(kmScalar position)
{
    Goto(position);
    Resume();
}

kmScalar CAnimationInstance::Position() const
{
    return m_fPosition;
}

bool CAnimationInstance::Destroyed() const
{
    return m_bDestroyed;
}

bool CAnimationInstance::isReverse() const
{
    return (static_cast<int>(m_replayMode) & static_cast<int>(CAnimation::EReplayMode::eRM_REVERSE)) != 0;
}

bool CAnimationInstance::isLoop() const
{
    return (static_cast<int>(m_replayMode) & ~static_cast<int>(CAnimation::EReplayMode::eRM_REVERSE)) 
        == static_cast<int>(CAnimation::EReplayMode::eRM_LOOP);
}

bool CAnimationInstance::isBounce() const
{
    return (static_cast<int>(m_replayMode) & ~static_cast<int>(CAnimation::EReplayMode::eRM_REVERSE)) 
        == static_cast<int>(CAnimation::EReplayMode::eRM_BOUNCE);
}
