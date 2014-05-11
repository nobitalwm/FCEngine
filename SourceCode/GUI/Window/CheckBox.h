#ifndef GUI_WINDOW_CHECKBOX_H__INCLUDE
#define GUI_WINDOW_CHECKBOX_H__INCLUDE

#include "Window.h"

namespace FCGUI
{
    class CCheckBox : public CWindow
    {
        DECLARE_REFLECT_GUID(CCheckBox, 0x303FB947, CWindow)
        DEFINE_WINDOW_TYPE(eWT_WINDOW_CHECKBOX);
    public:
        enum EEventType
        {
            eET_EVENT_BUTTON = FCGUI::eGUIET_EVENT_CHECKBOX,
            eET_EVENT_CHECK,
        };

        enum EStateType
        {
            eST_STATE_NORMAL_UNCHECKED,
            eST_STATE_NORMAL_CHECKED,
            eST_STATE_DISABLED_UNCHECKED,
            eST_STATE_DISABLED_CHECKED,
            eST_STATE_COUNT,
        };

    public:
        CCheckBox();
        CCheckBox( const TString &name );
        virtual ~CCheckBox();

        virtual void ReflectData(CSerializer& serializer) override;
        virtual bool OnMouseEvent( CMouseEvent *event ) override;
        bool         IsCheck() const;
        void         SetCheck( bool bCheck );
        virtual int  CurrState() const override;
        CWindow*      GetSubWindow();
    protected:
        void InitChild();
        void Init();
    private:
        bool m_bCheck;
        EStateType m_state;
        kmScalar m_fContentWidth;
        CWindow* m_pSubContent;
        TString m_strIndicatorNormalCheck;
        TString m_strIndicatorNormalUnCheck;
        TString m_strIndicatorDisabledCheck;
        TString m_strIndicatorDisabledUnCheck;
        TString m_strContentNormal;
        TString m_strContentDisabled;
    };
}

#endif
