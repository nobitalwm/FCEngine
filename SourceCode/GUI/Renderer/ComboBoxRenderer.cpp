#include "stdafx.h"
#include "ComboBoxRenderer.h"
#include "Render/TextureFragManager.h"
#include "ListBoxRenderer.h"
#include "GUI/Window/ListBox.h"
using namespace FCGUI;

CComboBoxRenderer::CComboBoxRenderer()
{

}

CComboBoxRenderer::~CComboBoxRenderer()
{

}

void CComboBoxRenderer::AddDropDownLayer(const TString &downTextureName, CButton::EStateType downState)
{
    CComboBox *pCombobox = static_cast<CComboBox*>(m_pWindow);
    CButton *pDropdownBtn = pCombobox->GetDropDownButtton();

    CBaseRenderer* pRenderer = pDropdownBtn->Renderer();
    pRenderer->AddLayer(downTextureName, downState);
}
void CComboBoxRenderer::AddComboBoxLayer(const TString &comboxTextureName, const TString &listboxTexureName, const TString &selTextureName)
{
    CBaseRenderer::AddLayer(comboxTextureName);
    CComboBox *pComboBox = static_cast<CComboBox*>(m_pWindow);
    CListBox *pListbox = pComboBox->GetListBox();

    CListBoxRenderer* pRenderer = static_cast<CListBoxRenderer*>(pListbox->Renderer());
    pRenderer->AddLayer(listboxTexureName,selTextureName);
}