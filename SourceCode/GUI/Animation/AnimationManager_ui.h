#ifndef GUI_ANIMATION_ANIMATION_MANAGER_H__INCLUDE
#define GUI_ANIMATION_ANIMATION_MANAGER_H__INCLUDE

#include "Animation_ui.h"

namespace FCGUI
{
    class CAnimationInstance;
    class CAnimatable;

    class CAnimationManager
    {
        BEATS_DECLARE_SINGLETON(CAnimationManager);
    public:
        void Update(kmScalar dt);

        CAnimation *CreateAnimation(const TString &name);
        void DeleteAnimation(const TString &name);

        CAnimationInstance *InstantiateAnimation(const TString &name, CAnimatable *target,
            CAnimation::EReplayMode replayMode = CAnimation::EReplayMode::eRM_DEFAULT, 
            kmScalar speed = 1.f, bool destroyOnStop = false);
        void DestroyInstance(CAnimationInstance *instance);

    private:
        std::map<TString, CAnimation *> m_animations;
        std::list<CAnimationInstance *> m_instances;
    };

}

#endif // !GUI_ANIMATION_ANIMATION_MANAGER_H__INCLUDE
