#include "stdafx.h"
#include "TextEditRenderer.h"
#include "Render/TextureFragManager.h"
#include "Render/TextureFrag.h"
#include "GUI/Font/FontManager.h"
#include "GUI/Font/FontFace.h"
#include "GUI/Window/TextEdit.h"
using namespace FCGUI;

CTextEditRenderer::CTextEditRenderer()
{

}

CTextEditRenderer::~CTextEditRenderer()
{

}

void CTextEditRenderer::RenderLayers( const kmMat4 &parentTransform ) const
{
    CBaseRenderer::RenderLayers(parentTransform);

    CTextEdit *edit = static_cast<CTextEdit*>(m_pWindow);
    BEATS_ASSERT(edit != nullptr);

    const TString&  text = edit->GetText();
    if(!text.empty())
    {
        kmVec3 pos; 
        kmVec3Fill( &pos, edit->RealPos().x, edit->RealPos().y, 0.f );

        const std::vector<TString> strVec = edit->GetTextOfLines();

        kmScalar padding = edit->GetPadding();
        pos.y += padding;
        
        kmScalar height = 0.f;
        for(size_t i = 0; i < strVec.size(); ++i)
        {
            const TString &str = strVec[i];
            if(i > 0)
            {
                height += edit->GetLineMaxHeight(i);
            }

            edit->GetFontFace()->RenderText(str, pos.x + padding, pos.y + height, 
                0x000000FF, &parentTransform);
        }
    }     
}