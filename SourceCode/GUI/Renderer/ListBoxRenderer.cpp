#include "stdafx.h"
#include "ListBoxRenderer.h"
#include "Render/TextureFragManager.h"
#include "Render/TextureFrag.h"
#include "GUI/Font/FontManager.h"
#include "GUI/Font/FontFace.h"
#include "GUI/Window/ListBox.h"
using namespace FCGUI;

CListBoxRenderer::CListBoxRenderer()
{

}

CListBoxRenderer::~CListBoxRenderer()
{

}

void  CListBoxRenderer::AddLayer(const TString &backTextName, const TString &selTextName)
{
    CListBox *listbox = static_cast<CListBox *>(m_pWindow); 
    CBaseRenderer::AddLayer(backTextName);

    CWindow *subSel = listbox->GetSubSelWin();
    CBaseRenderer* subrender = subSel->Renderer();
    subrender->AddLayer(selTextName);
}

void  CListBoxRenderer::AddLayer(CTextureFrag* backLayer, CTextureFrag *selLayer)
{
    CListBox *listbox = static_cast<CListBox *>(m_pWindow); 
    CBaseRenderer::AddLayer(backLayer);

    CWindow *subSel = listbox->GetSubSelWin();
    CBaseRenderer* subrender = subSel->Renderer();
    subrender->AddLayer(selLayer);
}
void CListBoxRenderer::RenderLayers( const kmMat4 &parentTransform ) const
{
    CBaseRenderer::RenderLayers(parentTransform);

    CListBox *listbox = static_cast<CListBox*>(m_pWindow);
    BEATS_ASSERT(listbox != nullptr);

    kmVec3 pos;
    kmVec3Fill(&pos,  listbox->RealPos().x, listbox->RealPos().y, 0.f);

    const std::vector<TString> strVec = listbox->GetStringList();

    kmScalar padding = listbox->GetPadding();
    pos.y += padding;

    kmScalar height = listbox->GetItemHeight();
    for(size_t i = 0; i < strVec.size(); ++i)
    {
        const TString &str = strVec[i];
        listbox->GetFontFace()->RenderText(str, pos.x + padding, pos.y + i * height, 
            0x000000FF, &parentTransform);
    }
}