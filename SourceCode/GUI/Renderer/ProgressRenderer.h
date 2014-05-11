#ifndef GUI_RENDERER_PROGRESS_RENDERER_H__INCLUDE
#define GUI_RENDERER_PROGRESS_RENDERER_H__INCLUDE

#include "BaseRenderer.h"
#include "GUI/Window/Progress.h"

class CTextureFrag;

namespace FCGUI
{
    class  CProgressRenderer : public CBaseRenderer
    {
        DEFINE_RENDERER_TYPE(RENDERER_PROGRESS)
    public:
        CProgressRenderer();
        virtual ~CProgressRenderer();

        void  AddLayer(const TString &fillTextName, const TString &backTextName, CProgress::EStateType state);
        void  AddLayer(CTextureFrag* fillLayer, CTextureFrag *backLayer, CProgress::EStateType state);
    };
}

#endif