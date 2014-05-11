#ifndef FCENGINEEDITOR_ENGINESPLASH_H__INCLUDE
#define FCENGINEEDITOR_ENGINESPLASH_H__INCLUDE

#include "wx/image.h"
#include "wx/splash.h"
#include "wx/mediactrl.h"

class CEngineSplash
{
public:
    CEngineSplash(wxBitmap* bitmap, long splashStyle, int milliseconds,
                    wxWindow* parent, wxWindowID id,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxSIMPLE_BORDER|wxFRAME_NO_TASKBAR|wxSTAY_ON_TOP);
    virtual ~CEngineSplash();
    wxBitmap* GetBitmap() const;
    void Instance();
private:
    wxBitmap*   m_pBitmap;
    int         m_nStyle;
    int         m_nSplashStyle;
    int         m_iMilliseconds;
    wxWindow*   m_pParent;
    wxWindowID  m_id;
    wxPoint     m_pos;
    wxSize      m_size;
    
};

#endif