#ifndef GUI_RENDERER_SLIDER_RENDERER_H__INCLUDE
#define GUI_RENDERER_SLIDER_RENDERER_H__INCLUDE

#include "BaseRenderer.h"
#include "GUI/Window/Slider.h"
#include "GUI/Window/Button.h"
class CTextureFrag;

namespace FCGUI
{
    class  CSliderRenderer : public CBaseRenderer
    {
        DEFINE_RENDERER_TYPE(RENDERER_SLIDER)
    public:
        CSliderRenderer();
        virtual ~CSliderRenderer();

        void AddLayer(CTextureFrag *thumbLayer, CTextureFrag *channelLayer, CSlider::EStateType state);
        void AddLayer(const TString &thumbFlagName, const TString &channelFlagName, CSlider::EStateType state);
    };
}

#endif