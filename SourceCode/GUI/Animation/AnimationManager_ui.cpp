#include "stdafx.h"
#include "AnimationManager_ui.h"
#include "AnimationInstance.h"

using namespace FCGUI;

CAnimationManager *CAnimationManager::m_pInstance = nullptr;

CAnimationManager::CAnimationManager()
{

}

CAnimationManager::~CAnimationManager()
{
    for(auto instance : m_instances)
    {
        BEATS_SAFE_DELETE(instance);
    }
    for(auto animation : m_animations)
    {
        BEATS_SAFE_DELETE(animation.second);
    }
}

void CAnimationManager::Update( kmScalar dt )
{
    FC_PERFORMDETECT_SCOPE(ePNT_Animation2D);

    for(auto itr = m_instances.begin(); itr != m_instances.end();)
    {
        if((*itr)->Destroyed())
        {
            BEATS_SAFE_DELETE(*itr);
            itr = m_instances.erase(itr);
        }
        else
        {
            (*itr)->Update(dt);
            ++itr;
        }
    }
}

CAnimation *CAnimationManager::CreateAnimation(const TString &name)
{
    BEATS_ASSERT(m_animations.find(name) == m_animations.end(), 
        _T("Animation with name:%s already existed"), name.c_str());
    CAnimation *animation = new CAnimation(name);
    m_animations[name] = animation;
    return animation;
}

void CAnimationManager::DeleteAnimation(const TString &name)
{
    auto itr = m_animations.find(name);
    if(itr != m_animations.end())
    {
        BEATS_SAFE_DELETE(itr->second);
        m_animations.erase(itr);
    }
}

CAnimationInstance *CAnimationManager::InstantiateAnimation(const TString &name, 
                                                          CAnimatable *target,
                                                          CAnimation::EReplayMode replayMode, 
                                                          kmScalar speed,
                                                          bool destroyOnStop)
{
    CAnimationInstance *instance = nullptr;
    auto itr = m_animations.find(name);
    if(itr != m_animations.end())
    {
        instance = new CAnimationInstance(itr->second, target, replayMode, speed, destroyOnStop);
        m_instances.push_back(instance);
    }
    return instance;
}

void CAnimationManager::DestroyInstance(CAnimationInstance *instance)
{
    auto itr = std::find(m_instances.begin(), m_instances.end(), instance);
    if(itr != m_instances.end())
    {
        m_instances.erase(itr);
    }
    BEATS_SAFE_DELETE(instance);
}

