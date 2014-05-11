#ifndef GUI_WINDOW_PROGRESS_H__INCLUDE
#define GUI_WINDOW_PROGRESS_H__INCLUDE
#include "Window.h"

namespace FCGUI
{
    class CProgress : public CWindow
    {
        DECLARE_REFLECT_GUID(CProgress, 0x496E5DAB, CWindow)
        DEFINE_WINDOW_TYPE(eWT_WINDOW_PROGRESS);

    public:
        CProgress();
        CProgress(const TString &name);
        virtual ~CProgress();

        virtual void ReflectData(CSerializer& serializer) override;
        void    SetRange(int nRange);
        int     GetRange() const;
        void    SetPos(int nPos);
        int     GetPos();
        int     SetStep(int nStep);
        void    Step();
        CWindow* GetSubWndFill();
        void    UpdateProgress();
    protected:
        void Init();
    private:
        CWindow      *m_pSubWndFill;

        int          m_nRange;
        int          m_nPos;
        int          m_nStep;

        TString m_strBackGroundName;
        TString m_strFillName;
    };
}

#endif