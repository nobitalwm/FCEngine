#ifndef GUI_RENDERER_TEXTEDIT_RENDERER_H__INCLUDE
#define GUI_RENDERER_TEXTEDIT_RENDERER_H__INCLUDE

#include "BaseRenderer.h"
class CTextureFrag;
class CFontFace;
namespace FCGUI
{
    class  CTextEditRenderer : public CBaseRenderer
    {
        DEFINE_RENDERER_TYPE(RENDERER_TEXTEDIT)
    public:
        CTextEditRenderer();
        virtual ~CTextEditRenderer();

    protected:
        virtual void RenderLayers( const kmMat4 &parentTransform ) const override;
    };
}

#endif