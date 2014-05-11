#ifndef GUI_WINDOW_COMBOBOX_H__INCLUDE
#define GUI_WINDOW_COMBOBOX_H__INCLUDE

#include "Window.h"

namespace FCGUI
{
    class CButton;
    class CTextLabel;
    class CListBox;
    class CComboBox : public CWindow
    {
        DECLARE_REFLECT_GUID(CComboBox, 0x32E30460, CWindow)
        DEFINE_WINDOW_TYPE(eWT_WINDOW_COMBOBOX);
    public:
        enum EEventType
        {
            eET_EVENT_COMBOBOX = FCGUI::eGUIET_EVENT_COMBOBOX,
            eET_EVENT_DROPDOWN,
            eET_EVENT_SELECTOK,
        };
        CComboBox();
        CComboBox( const TString &name );
        virtual ~CComboBox();

        virtual void ReflectData(CSerializer& serializer) override;
        int GetCurrSel( ) const;
        int SetCurrSel( int select );
        size_t AddString( const TString &strText);
        void DeleteString( size_t index );
        CButton* GetDropDownButtton() const;
        CListBox* GetListBox() const;
        CTextLabel* GetTextLabel() const;

        void OnDropDown(CBaseEvent *baseEvent);
        void OnSelectOk(CBaseEvent *baseEvent);
    protected:
        void IntiChild();
        void Init();
        void ReSizeChild();
    protected:
        CButton   *m_pDropDwonButton;
        CTextLabel *m_pSelectItem;
        CListBox *m_pListBox;
        kmScalar m_fDropDownButtonWidth;
        kmScalar m_fListBoxHeight;

        TString m_strDropDownBtnNormal;
        TString m_strDropDownBtnPressed;
        TString m_strDropDownBtnDisabled;
        TString m_strComboboxNormal;
        TString m_strComboboxDisabled;

        TString m_strListBackName;
        TString m_strListSelectName;
    };

}

#endif