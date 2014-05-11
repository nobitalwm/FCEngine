#ifndef GUI_RENDERER_COMBOBOX_RENDERER_H__INCLUDE
#define GUI_RENDERER_COMBOBOX_RENDERER_H__INCLUDE

#include "BaseRenderer.h"
#include "GUI/Window/ComboBox.h"
#include "GUI/Window/Button.h"
namespace FCGUI
{
    class  CComboBoxRenderer : public CBaseRenderer
    {
        DEFINE_RENDERER_TYPE(RENDERER_COMBOBOX)
    public:
        CComboBoxRenderer();
        virtual ~CComboBoxRenderer();
        void AddDropDownLayer(const TString &downTextureName, CButton::EStateType downState);
        void AddComboBoxLayer(const TString &comboxTextureName, const TString &listboxTexureName, const TString &selTextureName);
    };

}

#endif
