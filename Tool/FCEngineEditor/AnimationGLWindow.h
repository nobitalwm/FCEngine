#ifndef FCENGINEEDITOR_ANIMATIONGLWINDOW_H__INCLUDE
#define FCENGINEEDITOR_ANIMATIONGLWINDOW_H__INCLUDE

#include "FCEditorGLWindow.h"

class CModel;
class CAnimationGLWindow : public CFCEditorGLWindow
{
    typedef CFCEditorGLWindow super;
public:
    CAnimationGLWindow(wxWindow *parent,
                        wxGLContext* pShareContext = NULL,
                        wxWindowID id = wxID_ANY,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize, long style = 0,
                        const wxString& name = wxT("FCEditorGLWindow"));
    virtual ~CAnimationGLWindow();

    void SetModel(CModel* pModel);
    CModel* GetModel();

protected:
    virtual void OnTimer(wxTimerEvent& event);

private:
    CModel* m_pModel;
    DECLARE_EVENT_TABLE()
};
#endif