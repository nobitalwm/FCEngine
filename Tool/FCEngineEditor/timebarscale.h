#ifndef __SCALEBARCTRL_H
#define __SCALEBARCTRL_H

#include "wx/scrolwin.h"

class CTimeBarFrame;
class CTimeBarScale : public wxPanel
{
public:
    CTimeBarScale( wxWindow *parent );
    
    void                DrawScale();
    int                 PointToScale(wxPoint point);
    wxPoint             ScaleToPoint(int iScale);
    void                SetScaleCount(int iScaleCount);
    int                 GetScaleCount();
    void                SetTimeBarFrameWindow(CTimeBarFrame* pSplitterWindow);
    CTimeBarFrame*       GetTimeBarFrameWindow();

protected:
    void                OnPaint( wxPaintEvent& event );
    void                OnMouse( wxMouseEvent& event );

private:
    int     m_iCursorPositionX;
    CTimeBarFrame* m_pSplitterwnd;
    DECLARE_EVENT_TABLE()
};



#endif