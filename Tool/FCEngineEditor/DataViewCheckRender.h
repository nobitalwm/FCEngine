#ifndef __DATAVIEWCHECKRENDERER_H__INCLUDE
#define __DATAVIEWCHECKRENDERER_H__INCLUDE

#include "wx/wx.h"
#include <wx/dataview.h>

class CTimeBarCheckRenderer: public wxDataViewCustomRenderer
{
public:
    CTimeBarCheckRenderer(int type);
    virtual ~CTimeBarCheckRenderer();

    virtual bool Render( wxRect rect, wxDC *dc, int state );
    virtual bool ActivateCell(const wxRect& WXUNUSED(cell),
                                wxDataViewModel *model,
                                const wxDataViewItem &item,
                                unsigned int col,
                                const wxMouseEvent *mouseEvent);

    virtual wxSize GetSize() const;
    virtual bool SetValue( const wxVariant &value );
    virtual bool GetValue( wxVariant &WXUNUSED(value) ) const;

private:

    int         m_nType;
    int         m_nSelectedRow;
    wxRect      m_iconRect;
    wxString    m_strValue;
};
#endif