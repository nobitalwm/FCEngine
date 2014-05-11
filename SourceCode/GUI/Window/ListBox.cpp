#include "stdafx.h"
#include "ListBox.h"
#include "GUI/Event/WindowEvent.h"
#include "Event/MouseEvent.h"
#include "GUI/WindowManager.h"
#include "Render/TextureFragManager.h"
#include "GUI/Renderer/CheckBoxRenderer.h"
#include "GUI/Font/FontManager.h"

using namespace FCGUI;

CListBox::CListBox()
{
    Init();
}

CListBox::CListBox(const TString &name)
    : CWindow(name)
{
    Init();
}

CListBox::~CListBox()
{

}

void CListBox::ReflectData(CSerializer& serializer)
{
    CWindow::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_fPadding, true, 0xFFFFFFFF, _T("±ß¿ò´óÐ¡"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strBackGroundName, true, 0xFFFFFFFF, _T("±³¾°Í¼"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strSelectName, true, 0xFFFFFFFF, _T("Ñ¡ÖÐ±³¾°Í¼"), NULL, NULL, NULL);
}

bool CListBox::OnMouseEvent( CMouseEvent *event )
{
    bool bRet = false;
    if( IsEnabled() )
    {
        if(event->Button() == GLFW_MOUSE_BUTTON_LEFT)
        {
            if(event->Type() == eEVT_EVENT_MOUSE_PRESSED)
            {
                kmVec3 pos;
                kmVec3Fill( &pos, event->X(),  event->Y(), 0.f );
                CWindow::WorldToLocal( pos );

                if(IsContainPos(pos))
                {
                    int currSel = (int) (pos.y / m_fItemHeight );
                    if( currSel != m_nCurrSel && currSel >= 0 &&  (size_t)currSel < m_strList.size() )
                    {
                        m_nCurrSel = currSel;
                        WindowEvent event(eET_EVENT_SELCHANGE);
                        DispatchEvent(&event);

                        ShowSelBackGround();
                    }
                }
            }
        }
        bRet = true;
    }
    return bRet;
}

size_t CListBox::GetCount( ) const
{
    return m_strList.size();
}

kmScalar CListBox::GetItemHeight( ) const
{
    return m_fItemHeight;
}

int CListBox::GetCurrSel( ) const
{
    return m_nCurrSel;
}

int CListBox::SetCurrSel( int select )
{
    int oldCurrSel = m_nCurrSel;
    m_nCurrSel = select;

    ShowSelBackGround();
    return oldCurrSel;
}

size_t CListBox::AddString( const TString &strText)
{
    m_strList.push_back(strText);
    CaclItemHeight();
    return m_strList.size();
}

void CListBox::DeleteString( size_t index )
{
    if( index < m_strList.size() )
    {
        CaclItemHeight();
        m_strList.erase(m_strList.begin() + index);
    }
}

void CListBox::InsertString( int index, const TString &strText )
{
    m_strList.insert( m_strList.begin() + index, strText);
    CaclItemHeight();
}

const CListBox::TStringList& CListBox::GetStringList() const
{
    return m_strList;
}

kmScalar CListBox::GetPadding() const
{
    return m_fPadding;
}

CWindow*  CListBox::GetSubSelWin()
{
    return m_pSubSel;
}

TString CListBox::GetString(int index) const
{
    TString result = _T("");
    if( index >=0 && (size_t)index < m_strList.size())
    {
        result = m_strList[index];
    }
    return result;
}

void CListBox::Init()
{
    m_fItemHeight = 0.f;
    m_nCurrSel = -1;
    m_fPadding =  3.f;
    TCHAR selName[32];
    _stprintf_s(selName, _T("%p_SubSel"), this);
    m_pSubSel = CWindowManager::GetInstance()->Create<CWindow>(selName, this);
    m_pFontFace  = CFontManager::GetInstance()->GetFace(_T("STFANGSO_12"));
}

bool CListBox::IsContainPos(const kmVec3 &pos)
{
    kmVec2 size = RealSize();
    return (pos.x >=0 && pos.x <= size.x 
        && pos.y >=0 && pos.y <= size.y );
}

void CListBox::ShowSelBackGround()
{
    int count = m_strList.size();
    float percent = (float)m_nCurrSel / (float)count;
    GetSubSelWin()->SetArea(0.f,  0.f,  0.f,  percent, 0.f, m_fItemHeight, 1.f, percent);
}

void CListBox::CaclItemHeight()
{
    int count = m_strList.size();
    kmVec2 size = RealSize();
    m_fItemHeight = size.y / (float)count;
}

CFontFace* CListBox::GetFontFace() const
{
    return m_pFontFace;
}
