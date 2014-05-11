#ifndef __DATAVIEWLISTMODEL_H__INCLUDE
#define __DATAVIEWLISTMODEL_H__INCLUDE

#include <wx/dynarray.h>
#include <wx/dataview.h>

enum
{
    Col_EditableText,
    Col_Visible,
    Col_Lock,
    Col_Max
};

WX_DEFINE_ARRAY_CHAR(char, wxArrayChar);
class CTimeBarListModel: public wxDataViewVirtualListModel
{
public:
    CTimeBarListModel();
    virtual ~CTimeBarListModel();

    // helper methods to change the model
    void DeleteItem( unsigned int index );
    void DeleteItem( wxDataViewItem& index );
    void DeleteItems( const wxDataViewItemArray &items );
    void AddItem( const wxString text, bool visible, bool islock );

    // implementation of base class virtuals to define model
    virtual unsigned int    GetColumnCount() const;
    virtual wxString        GetColumnType( unsigned int col ) const;
    virtual void            GetValueByRow( wxVariant &variant, unsigned int row, unsigned int col ) const;
    virtual bool            SetValueByRow( const wxVariant &variant, unsigned int row, unsigned int col );

private:

    wxArrayString    m_textColValues;
    wxArrayString    m_iconColValues;
    wxArrayChar      m_view;
    wxArrayChar      m_lock;
};


#endif