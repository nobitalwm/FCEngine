#ifndef GUI_WINDOW_TEXTEDIT_H__INCLUDE
#define GUI_WINDOW_TEXTEDIT_H__INCLUDE

#include "Window.h"
#include "GUI/Font/FontFace.h"

namespace FCGUI
{
    class EditBoxImpl;
    class  CTextEdit : public CWindow
    {
        DECLARE_REFLECT_GUID(CTextEdit, 0x4849E949, CWindow)
        DEFINE_WINDOW_TYPE(eWT_WINDOW_TEXTEDIT);
    public:
        enum EEventType
        {
            eET_EVENT_TEXTEDIT = FCGUI::eGUIET_EVENT_TEXTEDIT,
            eET_EVENT_TEXTEDIT_BEGINEDIT,
            eET_EVENT_TEXTEDIT_VALUECHANGE,
            eET_EVENT_TEXTEDIT_ENDEDIT,
        };

        enum EModeType
        {
            eMT_MODE_TEXTEDIT,
            eMT_MODE_TEXTLABEL,
        };
        CTextEdit();
        CTextEdit( const TString &name );
        virtual ~CTextEdit();

        virtual void ReflectData(CSerializer& serializer) override;
        void  SetText( const TString &text );
        const TString& GetText() const;
        int   GetFontSize()const;
        void  SetFontSize(int sizeInPt);
        const TString& GetFontFileName()const;
        void  SetFontFileName(const TString& name);
        int   GetTextSize()const;
        virtual int CurrState() const override;
        virtual bool OnMouseEvent( CMouseEvent *event ) override;
        virtual bool OnKeyboardEvent(CKeyboardEvent *event);

        void InsertText(const TString &text, int nPos);
        void PositionTextPos(const kmVec3 &localPos);
        void DeleteBackward();
        
        size_t GetLineCount() const;
        kmScalar GetPadding() const;
        kmScalar GetLineMaxHeight(size_t indexLine) const;
        const std::vector<TString>& GetTextOfLines();
        void SetMode(const EModeType &mode);
        const EModeType& GetMode() const;
        CFontFace* GetFontFace() const;

    protected:
        void Init();
        void CalcLineLimit();
        void ProcessTextChange(TString strAdd);
        
        bool IsPassedRowLimit();
        bool IsContainPos(const kmVec3 &pos);
        void CalcStringOfLine();
        void AddText(TString strAdd);

    private:
        CFontFace    *m_pFontFace;
        EModeType m_mode;
        std::vector<TString>  m_textOfLine;
        TString   m_strText;
        bool      m_bEdit;
        bool      m_bInsert;

        int  m_nMaxLength;
        int  m_nCurrCharPos;
        size_t m_uLineCount;
        kmScalar m_fPadding;

        TString m_strBackGroundName;
    };
}

#endif