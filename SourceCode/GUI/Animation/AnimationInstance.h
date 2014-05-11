#ifndef GUI_ANIMATION_ANIMATION_INSTANCE_H__INCLUDE
#define GUI_ANIMATION_ANIMATION_INSTANCE_H__INCLUDE

#include "Animation_ui.h"
#include "Event/EventDispatcher.h"
#include "Event/EventType.h"

namespace FCGUI
{
    class CAnimation;

    class CAnimationInstance : public CEventDispatcher
    {
    public:
        enum EEventType
        {
            eET_EVENT_ANIMATION = ::eEVT_EVENT_ANIMATION,
            eET_EVENT_STARTED,
            eET_EVENT_STOPED,
            eET_EVENT_PAUSED,
            eET_EVENT_RESUMED,
            eET_EVENT_LOOPED,
            eET_EVENT_BOUNCED,
        };

        CAnimationInstance(CAnimation *animation, CAnimatable *target, 
            CAnimation::EReplayMode replayMode = CAnimation::EReplayMode::eRM_DEFAULT, 
            kmScalar speed = 1.0f, bool destroyOnStop = false);
        ~CAnimationInstance();

        void Update(kmScalar dt);

        void Play();
        void Pause();
        void Resume();
        void Stop();
        void Goto(kmScalar position);
        void GotoAndPlay(kmScalar position);
        void GotoAndPause(kmScalar position);

        kmScalar Position() const;

        bool Destroyed() const;

    private:
        bool isReverse() const;
        bool isLoop() const;
        bool isBounce() const;

    private:
        CAnimation *m_pAnimation;
        CAnimatable *m_pTarget;
        CAnimation::EReplayMode m_replayMode;
        kmScalar m_fSpeed;
        kmScalar m_fPosition;
        bool m_bBackward;
        bool m_bPlaying;
        bool m_bDestroyOnStop;
        bool m_bDestroyed;
    };

}

#endif // !GUI_ANIMATION_ANIMATION_INSTANCE_H__INCLUDE
