#ifndef GUI_WINDOW_TEXTLABEL_H__INCLUDE
#define GUI_WINDOW_TEXTLABEL_H__INCLUDE

#include "Window.h"
#include "GUI/Font/FontFace.h"

namespace FCGUI
{
    class CTextLabel : public CWindow
    {
        DECLARE_REFLECT_GUID(CTextLabel, 0xCAB44DF9, CWindow)
        DEFINE_WINDOW_TYPE(eWT_WINDOW_TEXTLABEL);
    public:
        enum EventType
        {
            EVENT_TEXTLABEL = FCGUI::eGUIET_EVENT_TEXTLABEL,
            EVENT_TEXTLABEL_PRESSED,
        };
        CTextLabel();
        CTextLabel(const TString &name);
        virtual ~CTextLabel();

        virtual void ReflectData(CSerializer& serializer) override;
        virtual bool OnMouseEvent( CMouseEvent *event ) override;
        const TString& GetDisplayText() const;
        void SetText(const TString &text);
        kmScalar GetPadding() const;
        CFontFace* GetFontFace() const;
    protected:
        void Init();
    private:
        CFontFace    *m_pFontFace;
        TString m_strDisplayText;
        kmScalar m_fPadding;
        TString m_strBackGroundName;
    };
}

#endif
