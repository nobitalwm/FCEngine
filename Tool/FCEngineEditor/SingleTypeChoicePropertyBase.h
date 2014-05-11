#ifndef FCENGINEEDITOR_SINGLETYPECHOICEPROPERTYBASE_H__INCLUDE
#define FCENGINEEDITOR_SINGLETYPECHOICEPROPERTYBASE_H__INCLUDE


#include <wx/propgrid/propgrid.h>

class wxSingleTypeChoiceDialogAdapterBase : public wxPGEditorDialogAdapter
{
public:

    wxSingleTypeChoiceDialogAdapterBase( const wxPGChoices& choices );

    virtual bool DoShowDialog( wxPropertyGrid* WXUNUSED(propGrid), wxPGProperty* WXUNUSED(property) );

protected:
    const wxPGChoices&  m_choices;
};


class CSingleTypeChoicePropertyBase : public wxFloatProperty
{
public:

    CSingleTypeChoicePropertyBase( const wxString& label,
        const wxString& name = wxPG_LABEL,
        double value = 0.0 );
    virtual ~CSingleTypeChoicePropertyBase();
    // Set editor to have button
    virtual const wxPGEditor* DoGetEditorClass() const;

    // Set what happens on button click
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const;

protected:
    int         m_iImageSizeX;
    int         m_iType;
    wxPGChoices m_choices;
};

#endif