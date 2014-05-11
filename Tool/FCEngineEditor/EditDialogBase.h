#ifndef FCENGINEEDITOR_EditDialogBase_H__INCLUDE
#define FCENGINEEDITOR_EditDialogBase_H__INCLUDE
#include <wx/dialog.h>

class CEditDialogBase : public wxDialog
{
    typedef wxDialog super;
public:
    CEditDialogBase(wxWindow *parent, wxWindowID id, const wxString &title
        , const wxPoint &pos = wxDefaultPosition
        , const wxSize &size = wxDefaultSize
        , long style =  wxRESIZE_BORDER | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX | wxCAPTION
        , const wxString &name = wxDialogNameStr);

    virtual ~CEditDialogBase();
    virtual int ShowModal();
    virtual void LanguageSwitch();
};

#endif