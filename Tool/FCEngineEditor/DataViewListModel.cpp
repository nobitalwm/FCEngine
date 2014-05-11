#include "stdafx.h"
#include "DataViewListModel.h"

CTimeBarListModel::CTimeBarListModel()
: wxDataViewVirtualListModel()
{

}

CTimeBarListModel::~CTimeBarListModel()
{

}

void CTimeBarListModel::DeleteItem( unsigned int index )
{
    if (index < m_textColValues.GetCount())
    {
        m_textColValues.RemoveAt( index );
        m_view.RemoveAt( index );
        m_lock.RemoveAt( index );
        RowDeleted( index );
    }
}

void CTimeBarListModel::DeleteItem( wxDataViewItem& item )
{
    unsigned int row = GetRow( item );
    DeleteItem( row );
}

void CTimeBarListModel::DeleteItems( const wxDataViewItemArray &items )
{
    wxArrayInt rows;
    for (unsigned i = 0; i < items.GetCount(); i++)
    {
        unsigned int row = GetRow( items[i] );
        if (row < m_textColValues.GetCount())
            rows.Add( row );
    }

    if (rows.GetCount() > 0)
    {
        // Sort in descending order so that the last
        // row will be deleted first. Otherwise the
        // remaining indeces would all be wrong.
        rows.Sort([](int *v1, int *v2)
        {
            return *v2-*v1;
        });
        for (unsigned i = 0; i < rows.GetCount(); i++)
        {
            m_textColValues.RemoveAt( rows[i] );
            m_view.RemoveAt( rows[i] );
            m_lock.RemoveAt( rows[i] );
        }
        // This is just to test if wxDataViewCtrl can
        // cope with removing rows not sorted in
        // descending order
        rows.Sort([](int *v1, int *v2)
        {
            return *v1-*v2;
        });
        RowsDeleted( rows );
    }
}

void CTimeBarListModel::AddItem( const wxString text, bool visible, bool islock )
{
    m_textColValues.push_back(text);
    m_view.push_back(visible);
    m_lock.push_back(islock);
    Reset(m_textColValues.Count());
}

void CTimeBarListModel::GetValueByRow( wxVariant &variant, unsigned int row, unsigned int col ) const
{
    if ((row < m_textColValues.GetCount()))
    {
        switch ( col )
        {
        case Col_EditableText:
            variant = m_textColValues[ row ];
            break;

        case Col_Visible:
            variant = m_view[row];
            break;

        case Col_Lock:
            variant = m_lock[row];
            break;

        case Col_Max:
            wxFAIL_MSG( _T("invalid column") );
            break;

        default:
            break;
        }
    }
}

bool CTimeBarListModel::SetValueByRow( const wxVariant &variant, unsigned int row, unsigned int col )
{
    if (row < m_textColValues.GetCount())
    {
        switch ( col )
        {
        case Col_EditableText:
            m_textColValues[row] = variant.GetString();
            break;

        case Col_Visible:
            m_view[row] = variant.GetChar();
            break;

        case Col_Lock:
            m_lock[row] = variant.GetChar();
            break;

        case Col_Max:
        default:
            wxFAIL_MSG( _T("invalid column") );
            break;
        }
    }
    return false;
}

unsigned int CTimeBarListModel::GetColumnCount() const
{
    return Col_Max;
}

wxString CTimeBarListModel::GetColumnType( unsigned int /*col*/ ) const
{
    return wxT("string");
}
