#ifndef GUI_RENDERER_RENDER_LAYER_H__INCLUDE
#define GUI_RENDERER_RENDER_LAYER_H__INCLUDE

#include "GUI/Animation/Animatable.h"

class CTextureFrag;
class CMaterial;

namespace FCGUI
{
    class CRenderLayer : public CAnimatable
    {
    public:
        typedef std::vector<CTextureFrag *> TFrameList;

        enum EProperty
        {
            PROP_CURR_FRAME,
        };

        CRenderLayer(const TString &textureAtlasName, const TString &textureFragName);
        CRenderLayer(CTextureFrag *textureFrag);
        CRenderLayer(const TFrameList &frames);
        virtual ~CRenderLayer();

        void Init();

        void SetCurrFrame(size_t currFrame);
        size_t CurrFrame() const;
        CTextureFrag *GetTextureFrag() const;
        SharePtr<CMaterial> GetMaterial() const;

    private:
        void InitAnimProp();

    private:
        TFrameList m_frames;
        size_t m_uCurrFrame;
        SharePtr<CMaterial> m_pMaterial;
    };
}

#endif // !GUI_RENDERER_RENDER_LAYER_H__INCLUDE
