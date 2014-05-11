#ifndef GUI_WINDOW_SLIDER_H__INCLUDE
#define GUI_WINDOW_SLIDER_H__INCLUDE

#include "Window.h"
#include "Button.h"

namespace FCGUI
{
    class  CSlider : public CWindow
    {
        DECLARE_REFLECT_GUID(CSlider, 0x983F7E61, CWindow)
        DEFINE_WINDOW_TYPE(eWT_WINDOW_SLIDER);
    public:
        enum EEventType
        {
            eET_EVENT_SLIDER = FCGUI::eGUIET_EVENT_SLIDER,
            eET_EVENT_MOVE,
        };

        enum ESliderType
        {
            eST_SLIDER_TYPE_HORIZONTAL,
            eST_SLIDER_TYPE_VERTICAL,
        };

    public:
        CSlider();
        CSlider(const TString &name);
        virtual ~CSlider();

        virtual void ReflectData(CSerializer& serializer) override;
        void SetRange(int nMin, int nMax);
        void SetMinRange(int nMin);
        void SetMaxRange(int nMax);
        void GetRange(int& nMin, int nMax);
        void SetPos(int nPos);
        int GetPos();
        void SetThumbSize(kmVec2 size);
        kmVec2 GetThumbSize() const; 
        CButton* GetSubThumb();
        ESliderType GetSliderType();
        void SetSliderType(ESliderType type);
        void UpdatePos();
        virtual bool OnMouseEvent( CMouseEvent *event ) override;

    protected:
        void Init();
        bool IsContainPos(const kmVec3 &pos);
    private:
        ESliderType  m_slierType;
        CButton*    m_pSubThumb;

        int m_nMinRange;
        int m_nMaxRange;

        int m_nPos;
        bool m_bPressed;

        kmVec2 m_thumbSize;

        TString m_strThumbNormal;
        TString m_strThumbPressed;
        TString m_strThumbDisabled;
        TString m_strChanNormal;
        TString m_strChanDisable;
        TString m_strChanFillNormal;
        TString m_strChanFillDisabled;
    };

}

#endif

