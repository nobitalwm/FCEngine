#include "stdafx.h"
#include "TextLabelRenderer.h"
#include "Render/TextureFragManager.h"
#include "Render/TextureFrag.h"
#include "GUI/Font/FontManager.h"
#include "GUI/Font/FontFace.h"
#include "GUI/Window/TextLabel.h"
using namespace FCGUI;

CTextLabelRenderer::CTextLabelRenderer()
{

}

CTextLabelRenderer::~CTextLabelRenderer()
{

}

void CTextLabelRenderer::RenderLayers( const kmMat4 &parentTransform ) const
{
    CBaseRenderer::RenderLayers(parentTransform);

    CTextLabel *textLabel = static_cast<CTextLabel*>(m_pWindow);
    BEATS_ASSERT(textLabel != nullptr);

    const TString&  text = textLabel->GetDisplayText();
    if(!text.empty())
    {
        kmScalar x = textLabel->RealPos().x + textLabel->GetPadding();
        kmScalar y = textLabel->RealPos().y + textLabel->GetPadding();

        textLabel->GetFontFace()->RenderText(text, x, y, 0x000000FF, &parentTransform);
    }     
}