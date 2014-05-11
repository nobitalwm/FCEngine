#ifndef GUI_RENDERER_CHECKBOX_RENDERER_H__INCLUDE
#define GUI_RENDERER_CHECKBOX_RENDERER_H__INCLUDE

#include "BaseRenderer.h"
#include "GUI/Window/CheckBox.h"

class CTextureFrag;

namespace FCGUI
{
    class  CCheckBoxRenderer : public CBaseRenderer
    {
        DEFINE_RENDERER_TYPE(RENDERER_CHECKBOX)
    public:
        CCheckBoxRenderer();
        virtual ~CCheckBoxRenderer();

        void AddLayer(const TString &contentTextureName, const TString &indicatorTextureName, CCheckBox::EStateType state);
        void AddLayer(CTextureFrag *contentLayer, CTextureFrag* indicatorLayer, CCheckBox::EStateType state);
    };
}



#endif