#ifndef GUI_WINDOW_LISTBOX_H__INCLUDE
#define GUI_WINDOW_LISTBOX_H__INCLUDE

#include "Window.h"
#include "GUI/Font/FontFace.h"
namespace FCGUI
{
    class CListBox : public CWindow
    {
        typedef std::vector<TString> TStringList;
        DECLARE_REFLECT_GUID(CListBox, 0x341E6B07, CWindow)
        DEFINE_WINDOW_TYPE(eWT_WINDOW_LISTBOX);

    public:
        enum EEventType
        {
            eET_EVENT_LISTBOX = FCGUI::eGUIET_EVENT_LISTBOX,
            eET_EVENT_SELCHANGE,
        };
        CListBox();
        CListBox(const TString &name);
        virtual ~CListBox();

        virtual void ReflectData(CSerializer& serializer) override;
        virtual bool OnMouseEvent( CMouseEvent *event ) override;

        size_t GetCount( ) const;
        kmScalar GetItemHeight( ) const;
        int GetCurrSel( ) const;
        int SetCurrSel( int select );
        size_t AddString( const TString &strText);
        void DeleteString( size_t index );
        void InsertString( int index, const TString &strText );
        const TStringList& GetStringList() const;
        kmScalar GetPadding() const;
        CWindow*  GetSubSelWin();
        TString GetString(int index) const; 
        CFontFace* GetFontFace() const;
    protected:
        void Init();
        bool IsContainPos(const kmVec3 &pos);
        void ShowSelBackGround();
        void CaclItemHeight();

    private:
        CFontFace *m_pFontFace;
        CWindow *m_pSubSel;
        kmScalar m_fItemHeight;
        int m_nCurrSel;
        TStringList m_strList;

        kmScalar m_fPadding;

        TString m_strBackGroundName;
        TString m_strSelectName;
    };
}
#endif