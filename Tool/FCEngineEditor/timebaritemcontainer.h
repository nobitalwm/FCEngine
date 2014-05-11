#ifndef __MYCANVAS_H
#define __MYCANVAS_H

#include "wx/wx.h"
#include "timebaritem.h"

class CTimeBarDataViewCtrl;
class CTimeBarFrame;
class CTimeBarScale;
class CTimeBarItem;

struct SSelectCtrl
{
    int m_iSelectRowBegin;
    int m_iSelectRowEnd;
    int m_iSelectColumnBegin;
    int m_iSelectColumnEnd;
};

class CTimeBarItemContainer: public wxScrolledWindow
{
    typedef wxScrolledWindow super;
public:
    CTimeBarItemContainer(wxWindow* parent);
    virtual ~CTimeBarItemContainer();

    virtual void        ScrollWindow(int x, int y, const wxRect *rect = NULL);
    void                SyncWith(CTimeBarDataViewCtrl* pDataViewCtrl, CTimeBarScale* pScaleBarCtrl);
    void                AddItem(CTimeBarItem* pitem);
    void                RefreshItems();
    void                SelectItems();
    void                DeleteItem(int index);
    void                SetCellWidth(int width);
    void                SetItemCellsCount(int iCount);
    void                SetTimeBarFrameWindow(CTimeBarFrame* pSplitterWindow);
    int                 GetCellWidth() const;
    SSelectCtrl&        GetCurrentSelect();
    ArrayOfTimeBarItem* GetLineBarItems();
    CTimeBarFrame*       GetTimeBarFrameWindow();

protected:
    void                OnSize(wxSizeEvent & event);

private:
    int                         m_iCellWidth;
    int                         m_iCursorPositionX;
    int                         m_iItemCellsCount;
    wxBoxSizer*                 m_pMainSizer;
    CTimeBarDataViewCtrl*        m_pSyncWnd;
    CTimeBarScale*               m_pSyncScale;
    CTimeBarFrame*               m_pSplitterwnd;
    ArrayOfTimeBarItem          m_items;
    SSelectCtrl                 m_currentSelect;
    SSelectCtrl                 m_lastSelect;

    wxDECLARE_NO_COPY_CLASS(CTimeBarItemContainer);
};


#endif