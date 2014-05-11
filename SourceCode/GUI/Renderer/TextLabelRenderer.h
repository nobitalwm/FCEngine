#ifndef GUI_RENDERER_TEXTLABEL_RENDERER_H__INCLUDE
#define GUI_RENDERER_TEXTLABEL_RENDERER_H__INCLUDE

#include "BaseRenderer.h"
class CTextureFrag;

namespace FCGUI
{
    class  CTextLabelRenderer : public CBaseRenderer
    {
        DEFINE_RENDERER_TYPE(RENDERER_TEXTLABEL)
    public:
        CTextLabelRenderer();
        virtual ~CTextLabelRenderer();

    protected:
        virtual void RenderLayers( const kmMat4 &parentTransform ) const override;
    };
}

#endif