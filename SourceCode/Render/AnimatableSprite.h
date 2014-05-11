#ifndef RENDER_ANIMATABLE_SPRITE_H__INCLUDE
#define RENDER_ANIMATABLE_SPRITE_H__INCLUDE

#include "Sprite.h"
#include "GUI/Animation/Animatable.h"

class CAnimatableSprite : public CSprite, public FCGUI::CAnimatable
{
public:
    typedef std::vector<CTextureFrag *> TFrameList;

    enum EAnimatableProperty
    {
        eAP_PROP_CURR_FRAME,
    };

    CAnimatableSprite(CTextureFrag *textureFrag);
    CAnimatableSprite(const TString &textureAtlasName, const TString &textureFragName);
    CAnimatableSprite(const TString &textureAtlasName, const TString &textureFragName, 
        const kmVec2 &size);
    CAnimatableSprite(const TString &textureAtlasName, const TString &textureFragName, 
        const kmVec2 &size, const kmVec2 &origin);
    virtual ~CAnimatableSprite();

    void SetFrames(const TFrameList &frames);
    void AddFrame(CTextureFrag *frame);
    const TFrameList &Frames() const;
    void SetCurrFrame(size_t currFrame);
    size_t CurrFrame() const;

    virtual void Render() override;

private:
    void InitAnimProp();

private:
    TFrameList m_frames;
    size_t m_uCurrFrame;
};

#endif