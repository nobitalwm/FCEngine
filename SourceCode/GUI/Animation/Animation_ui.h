#ifndef GUI_ANIMATION_ANIMATION_H__INCLUDE
#define GUI_ANIMATION_ANIMATION_H__INCLUDE


namespace FCGUI
{
    class CAnimationLayer;
    class CAnimatable;

    class CAnimation
    {
    public:
        enum class EReplayMode
        {
            eRM_DEFAULT,
            eRM_ONCE,
            eRM_LOOP,
            eRM_BOUNCE,
            eRM_REVERSE = 0x4,
            eRM_REVERSE_LOOP = eRM_REVERSE | eRM_LOOP,
            eRM_REVERSE_BOUNCE = eRM_REVERSE | eRM_BOUNCE,
        };

        CAnimation(const TString &name);

        ~CAnimation();

        const TString& Name() const;

        void SetDuration(kmScalar duration);
        kmScalar Duration() const;

        void SetMode(EReplayMode replayMode);
        EReplayMode Mode() const;

        CAnimationLayer *CreateLayer(const TString &name = _T(""));
        void DeleteLayer(CAnimationLayer *layer);

        void Apply(CAnimatable *target, kmScalar position);

    private:
        TString m_strName;
        kmScalar m_fDuration;
        EReplayMode m_replayMode;
        std::vector<CAnimationLayer *> m_layers;
    };

}

#endif // !GUI_ANIMATION_ANIMATION_H__INCLUDE
