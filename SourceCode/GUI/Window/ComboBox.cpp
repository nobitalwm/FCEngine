#include "stdafx.h"
#include "Button.h"
#include "TextLabel.h"
#include "ListBox.h"
#include "ComboBox.h"
#include "GUI/Event/WindowEvent.h"
#include "Event/MouseEvent.h"
#include "GUI/WindowManager.h"

using namespace FCGUI;

CComboBox::CComboBox()
{
    Init();
}

CComboBox::CComboBox(const TString &name)
    : CWindow(name)
{
    Init();
}

CComboBox::~CComboBox()
{

}

void CComboBox::ReflectData(CSerializer& serializer)
{
    CWindow::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_fDropDownButtonWidth, true, 0xFFFFFFFF, _T("下拉按钮宽度"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_fListBoxHeight, true, 0xFFFFFFFF, _T("列表框高度"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strDropDownBtnNormal, true, 0xFFFFFFFF, _T("下拉按钮图"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strDropDownBtnPressed, true, 0xFFFFFFFF, _T("下拉按钮图按下"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strDropDownBtnDisabled, true, 0xFFFFFFFF, _T("下拉按钮图禁用"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strComboboxNormal, true, 0xFFFFFFFF, _T("组合框图"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strComboboxDisabled, true, 0xFFFFFFFF, _T("组合框图禁用"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strListBackName, true, 0xFFFFFFFF, _T("下拉列表图"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strListSelectName, true, 0xFFFFFFFF, _T("下拉列表选中图"), NULL, NULL, NULL);
}

void CComboBox::IntiChild()
{
    TCHAR dropDwonName[32];
    _stprintf_s(dropDwonName, _T("%p_SubDropDown"), this);
    m_pDropDwonButton = CWindowManager::GetInstance()->Create<CButton>(dropDwonName, this);
    m_pDropDwonButton->SubscribeEvent(CButton::eET_EVENT_CLICKED, &CComboBox::OnDropDown, this);
    m_pDropDwonButton->SetArea( - m_fDropDownButtonWidth, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f);

    TCHAR listBoxName[32];
    _stprintf_s(listBoxName, _T("%p_SubListbox"), this);
    m_pListBox = CWindowManager::GetInstance()->Create<CListBox>(listBoxName, this);
    m_pListBox->SetArea( 0.f, 0.f, 0.f, 1.f, 0.f, m_fListBoxHeight, 1.f, 1.f );
    m_pListBox->SubscribeEvent(CListBox::eET_EVENT_SELCHANGE, &CComboBox::OnSelectOk, this);
    m_pListBox->Hide();

    TCHAR selectItemName[32];
    _stprintf_s(selectItemName, _T("%p_SubTextLabel"), this);
    m_pSelectItem = CWindowManager::GetInstance()->Create<CTextLabel>(selectItemName, this);
    m_pSelectItem->SetArea( 0.f, 0.f, 0.f, 0.f, - m_fDropDownButtonWidth, 0.f, 1.f, 1.f );
}

void CComboBox::Init()
{
    m_fDropDownButtonWidth = 20.f;
    m_fListBoxHeight = 100.f;
    IntiChild();
}

size_t CComboBox::AddString( const TString &strText)
{
    return m_pListBox->AddString(strText);
}

int CComboBox::SetCurrSel( int select )
{
    return m_pListBox->SetCurrSel(select);
}

void CComboBox::DeleteString( size_t index )
{
    return m_pListBox->DeleteString(index);
}

 int CComboBox::GetCurrSel( ) const
 {
     return m_pListBox->GetCurrSel();
 }

CButton* CComboBox::GetDropDownButtton() const
{
    return m_pDropDwonButton;
}

CListBox* CComboBox::GetListBox() const
{
    return m_pListBox;
}

CTextLabel* CComboBox::GetTextLabel() const
{
    return m_pSelectItem;
}

void CComboBox::OnDropDown(CBaseEvent *baseEvent)
{
    if(m_pListBox->IsVisible())
    {
        m_pListBox->Hide();
    }
    else
    {
        m_pListBox->Show();
    }

    WindowEvent event(eET_EVENT_DROPDOWN);
    DispatchEvent(&event);
}

void CComboBox::OnSelectOk(CBaseEvent *baseEvent)
{
    m_pListBox->Hide();

    int currSel = m_pListBox->GetCurrSel();
    TString strText = m_pListBox->GetString(currSel);
    m_pSelectItem->SetText(strText);

    WindowEvent event(eET_EVENT_SELECTOK);
    DispatchEvent(&event);
}
