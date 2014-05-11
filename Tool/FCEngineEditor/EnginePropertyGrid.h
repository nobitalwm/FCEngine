#ifndef FCENGINEEDITOR_ENGINEPROPERTYGRID_H__INCLUDE
#define FCENGINEEDITOR_ENGINEPROPERTYGRID_H__INCLUDE

#include <wx/propgrid/propgrid.h>

class CEnginePropertyGrid : public wxPropertyGrid
{
public:
    CEnginePropertyGrid();
    virtual ~CEnginePropertyGrid();
    void FreeEditorCtrl();
    void SetManager(wxPropertyGridManager* pManager);
    wxPropertyGridManager* GetManager() const;
private:
    wxPropertyGridManager* m_pManager;
};


#endif