#ifndef GUI_ANIMATION_ANIMATION_LAYER_H__INCLUDE
#define GUI_ANIMATION_ANIMATION_LAYER_H__INCLUDE

#include "KeyFrame.h"

namespace FCGUI
{
    class CAnimatable;
    class CAnimation;

    class CAnimationLayer
    {
    public:
        CAnimationLayer(const TString &name = _T(""));

        ~CAnimationLayer();

        void SetAnimation(CAnimation *animation);

        void SetEnabled(bool enabled);
        bool Enabled() const;

        void AddPropID(int propID);
        void DeletePropID(int propID);

        CKeyFrame *CreateKeyFrame(kmScalar position);
        void DeleteKeyFrame(CKeyFrame *keyFrame);
        
        void Apply(CAnimatable *target, kmScalar position);

    private:
        CKeyFrame::TValueList interpolation(const CKeyFrame *left, const CKeyFrame *right, kmScalar position);

    private:
        CAnimation *m_pAnimation;
        TString m_strName;
        bool m_bEnabled;
        std::vector<int> m_propIDs;
        std::vector<CKeyFrame *> m_keyFrames;
    };
}

#endif // !GUI_ANIMATION_ANIMATION_LAYER_H__INCLUDE
