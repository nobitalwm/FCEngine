#ifndef FCENGINEEDITOR_UIGLWINDOW_H__INCLUDE
#define FCENGINEEDITOR_UIGLWINDOW_H__INCLUDE

#include "FCEditorGLWindow.h"
#include "GUI\Window\Button.h"

class FCGUI::CWindow;
class CUIGLWindow : public CFCEditorGLWindow
{
    typedef CFCEditorGLWindow super;
public:
    CUIGLWindow(wxWindow *parent,
        wxGLContext* pShareContext = NULL,
        wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxString& name = wxT("FCEditorGLWindow"));
    virtual ~CUIGLWindow();

    void SetTempGUI(FCGUI::CWindow* pTempGUI);
    void SetActiveGUI(FCGUI::CWindow* pActiveGUI);
protected:
    virtual void OnTimer(wxTimerEvent& event);
    void RenderTempGUI();
    void OnChar(wxKeyEvent& event);
    void OnMouse(wxMouseEvent& event);

private:
    FCGUI::CWindow*          m_pActiveGUI;
    FCGUI::CWindow*          m_pTempGUI;
    wxPoint                  m_activeGUIPosOffset;
    DECLARE_EVENT_TABLE()
};
#endif