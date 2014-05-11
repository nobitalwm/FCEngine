#ifndef __DATAVIEWCTRL_H_INCLUDE
#define __DATAVIEWCTRL_H_INCLUDE

#include "wx/dataview.h"
#include "timebarevent.h"
#include <wx/dynarray.h>

#define CELLWIDTH 10
#define CELLHIGHT 23
#define SCROLLUNITSX 80
#define SCROLLUNITSY 4
#define INITCELLNUM 300
#define SCALEBARHIGHT 24
#define SCALECELLWIDTH 10
#define CURSORWIDTH 2
#define DEFAULTSCROLLBARWIDTH 17

struct STimeLineBarItemAnimation
{
    int     m_iStartFrame;
    int     m_iEndFrame;
    int     m_iFrameID;
};

WX_DECLARE_OBJARRAY(STimeLineBarItemAnimation, ArrayOfAnimation);

class CTimeBarListModel;
class CDataViewTreeModel;
class CDataViewTreeModelNode;
class CTimeBarDataViewCtrl : public wxDataViewCtrl
{
    typedef wxDataViewCtrl super;
public:
    CTimeBarDataViewCtrl( wxWindow *parent, wxWindowID id,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxDataViewCtrlNameStr );
    virtual ~CTimeBarDataViewCtrl();

    void            SyncWith( wxScrollHelperBase *win );
    int             GetRow( const wxDataViewItem & item );
    void            AddItem( const wxString &text, bool visible, bool islock );
    void            AppendItem(CDataViewTreeModelNode* pParent, const wxString &text, bool bVisible, bool bIslock );
    bool            InitColumn();
    void            RefreshSize();
    void            AssociateListModel();
    void            AssociateTreeModel();
    CTimeBarListModel*    GetUserListModel()const;
    CDataViewTreeModel*   GetUserTreeModel()const;
protected:
    void OnScroll(wxScrollWinEvent& event);

private:
    wxString            m_strItemName;
    wxScrollHelperBase* m_pSyncWnd;
    CTimeBarListModel*  m_pListModel;
    CDataViewTreeModel* m_pTreeModel;

    DECLARE_EVENT_TABLE()
};

#endif



