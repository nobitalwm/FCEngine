#ifndef FCENGINEEDITOR_FCEDITORGLWINDOW_H__INCLUDE
#define FCENGINEEDITOR_FCEDITORGLWINDOW_H__INCLUDE

#include "wx/glcanvas.h"

class CCamera;
class CFCEditorGLWindow : public wxGLCanvas
{
public:
    CFCEditorGLWindow(wxWindow *parent,
                    wxGLContext* pShareContext = NULL,
                    wxWindowID id = wxID_ANY,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize, long style = 0,
                    const wxString& name = wxT("FCEditorGLWindow"));

    virtual ~CFCEditorGLWindow();
    wxGLContext* GetGLContext() const;
    void SetGLContext(wxGLContext* pContext);
    virtual void OnTimer(wxTimerEvent& event);
    bool SafeGetKeyStage(int nVirtKey);

protected:
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnMouse(wxMouseEvent& event);
    void OnMouseMidScroll(wxMouseEvent& event);
    void ShowCursor();
    void HideCursor();

protected:
    bool m_bLeftDown;
    bool m_bRightDown;

private:
    void InitGL();
    void UpdateCamera();

private:
    wxPoint         m_startPosition;
    CCamera*        m_pCamera;
    wxGLContext*    m_glRC;

    wxDECLARE_NO_COPY_CLASS(CFCEditorGLWindow);
    DECLARE_EVENT_TABLE()
};
#endif