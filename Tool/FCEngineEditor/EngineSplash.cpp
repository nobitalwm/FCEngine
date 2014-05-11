#include "stdafx.h"
#include "EngineSplash.h"

CEngineSplash::CEngineSplash(wxBitmap* bitmap, long splashStyle, int milliseconds,
                           wxWindow* parent, wxWindowID id,
                           const wxPoint& pos ,
                           const wxSize& size ,
                           long style) :
                    m_nSplashStyle(splashStyle), m_iMilliseconds(milliseconds), m_pParent(parent), m_id(id), m_pos(pos), m_size(size), m_nStyle(style)
{
    if (bitmap->IsOk())
    {
        m_pBitmap = bitmap;
    }
    else
    {
        m_pBitmap = NULL;
    }
}

CEngineSplash::~CEngineSplash()
{
}

wxBitmap* CEngineSplash::GetBitmap() const
{
    return m_pBitmap;
}

void CEngineSplash::Instance()
{
    wxBitmap bmp;
    if (m_pBitmap != NULL)
    {
        bmp = *m_pBitmap;
    }
    
    wxSplashScreen* Splash = new wxSplashScreen(bmp, m_nSplashStyle, m_iMilliseconds, m_pParent, m_id, m_pos, m_size, m_nStyle);
    wxWindow *win = Splash->GetSplashWindow();
    wxStaticText *text = new wxStaticText( win,
        wxID_EXIT,
        wxT("FCEngineEditor V_0_1"),
        wxPoint(13, 11)
        );

    text->SetBackgroundColour(*wxWHITE);
    text->SetForegroundColour(*wxBLACK);
    wxFont font = text->GetFont();
    font.SetPointSize(2*font.GetPointSize()/3);
    text->SetFont(font);
}