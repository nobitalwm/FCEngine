#include "stdafx.h"
#include "SliderRenderer.h"
#include "BaseRenderer.h"
#include "Render/TextureFragManager.h"
#include "Render/TextureFrag.h"
using namespace FCGUI;

CSliderRenderer::CSliderRenderer()
{
   
}

CSliderRenderer::~CSliderRenderer()
{

}

void CSliderRenderer::AddLayer(const TString &thumbFlagName, const TString &channelFlagName, CSlider::EStateType state)
{
    CSlider *slider = static_cast<CSlider *>(m_pWindow);
    CBaseRenderer::AddLayer(channelFlagName,state);

    CButton *subThumb = slider->GetSubThumb();
    CBaseRenderer* sliderrender = static_cast<CBaseRenderer*>(subThumb->Renderer());
    sliderrender->AddLayer(thumbFlagName,state);
}

void CSliderRenderer::AddLayer(CTextureFrag *thumbLayer, CTextureFrag *channelLayer, CSlider::EStateType state)
{
    CSlider *slider = static_cast<CSlider *>(m_pWindow);
    CBaseRenderer::AddLayer(channelLayer,state);

    CButton *subThumb = slider->GetSubThumb();
    CBaseRenderer* sliderrender = static_cast<CBaseRenderer*>(subThumb->Renderer());
    sliderrender->AddLayer(thumbLayer,state);
}




