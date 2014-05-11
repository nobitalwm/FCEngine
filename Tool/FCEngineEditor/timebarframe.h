#ifndef __SPLITER_H
#define __SPLITER_H

#include "wx/splitter.h"
#include "wx/scrolwin.h"
#include "timebaritemcontainer.h"
#include "timebarscale.h"
#include "wx/button.h"
#include "DataViewCtrl.h"
#include <wx/spinctrl.h>

#define FREQUENCYREDUCTIONFACTOR 4
#define DIFFERENTDRAWLENGTH 5
    
class CAnimation;
class CTimeBarDataViewCtrl;
class CTimeBarItemContainer;
class wxSpinCtrlDouble;
class CTimeBarFrame : public wxSplitterWindow
{
    typedef wxSplitterWindow super;
public:
    CTimeBarFrame(wxWindow *parent);
    virtual ~CTimeBarFrame();

    void            ResetLeft();
    void            AddItems();
    void            AddItems(SharePtr<CAnimation> pAnimationItem);
    void            DeleteItems(int index);
    void            ClickOnScaleBar();
    void            SetCursorPositionX(int pos);
    int             GetCursorPositionX();
    void            SetCurrentCursor(int pos);
    void            SetCurrentSelect(int iCurrentSelect);
    int             GetCurrentSelect() const;
    void            SetClickOnScalebar(bool bIsClicked);
    bool            IsClickOnScalebar() const;
    void            SetFps(size_t iFps);
    void            SetFpsToAnimation(size_t iFps);
    void            SetFrameWindow(wxWindow* pWindow);
    int             GetItemCount();
    int             GetCellWidth();
    wxWindow*       GetFrameWindow();
    CTimeBarScale*   GetScalebar();
    CTimeBarItemContainer*   GetItemContainer();

protected:
    void OnSize(wxSizeEvent& event);
    void OnBtnAddClick(wxCommandEvent& event);
    void OnBtnDeleteClick(wxCommandEvent& event);
    void OnPositionChanged(wxSplitterEvent& event);
    void OnPositionChanging(wxSplitterEvent& event);
    void OnSpinCtrl(wxSpinEvent& event);
    void OnSpinTextEnter(wxCommandEvent& event);
    void OnSelect(wxDataViewEvent& event);

private:
    int                     m_iCellWidth;
    int                     m_iItemId;
    int                     m_iCurrentSelect;
    int                     m_iCursorPositionX;
    bool                    m_bclickonscalebar;
    CTimeBarDataViewCtrl*   m_pTimeLineDataViewCtrl;
    CTimeBarItemContainer*   m_pContainer;
    CTimeBarScale*           m_pScalebar;
    wxButton*               m_pButtonAdd;
    wxButton*               m_pButtonDelete;
    wxSpinCtrl*             m_pSpinCtrl;
    wxWindow*               m_pFrame;

    DECLARE_EVENT_TABLE()
    wxDECLARE_NO_COPY_CLASS(CTimeBarFrame);
};

#endif