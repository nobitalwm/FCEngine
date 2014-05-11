#include "stdafx.h"
#include "TextEdit.h"
#include "GUI/Event/WindowEvent.h"
#include "Event/MouseEvent.h"
#include "Event/KeyboardEvent.h"
#include "GUI/WindowManager.h"
#include "GUI/Font/FontManager.h"
#include "Utility/BeatsUtility/StringHelper.h"

using namespace FCGUI;

CTextEdit::CTextEdit()
{
    Init();
}

CTextEdit::CTextEdit(const TString &name)
    : CWindow(name)
{
    Init();
}

CTextEdit::~CTextEdit()
{

}

void CTextEdit::ReflectData(CSerializer& serializer)
{
    CWindow::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_fPadding, true, 0xFFFFFFFF, _T("±ß¿ò´óÐ¡"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strBackGroundName, true, 0xFFFFFFFF, _T("±³¾°Í¼"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_mode, true, 0xFFFFFFFF, _T("±à¼­/ÎÄ±¾±êÇ©"), NULL, NULL, NULL);
}

void CTextEdit::Init()
{
    m_nCurrCharPos = 0;
    m_uLineCount = 0;
    m_bInsert = false;
    m_fPadding = 4.f;
    m_mode = eMT_MODE_TEXTEDIT;
    m_pFontFace  = CFontManager::GetInstance()->GetFace(_T("STFANGSO_12"));
}

int CTextEdit::CurrState() const
{
    return 0;
}

int   CTextEdit::GetTextSize()const
{
    return m_strText.size();
}

void CTextEdit::SetText(const TString &text)
{
    if(!text.empty())
    {
        m_strText = text;
    }
}

const TString& CTextEdit::GetText() const
{
    return m_strText;
}

void CTextEdit::InsertText(const TString &text, int nPos)
{
    m_strText.insert(nPos,text);
}

void CTextEdit::DeleteBackward()
{
    if(!m_strText.empty() && m_nCurrCharPos > 0)
    {
        BEATS_ASSERT( (size_t)m_nCurrCharPos <= m_strText.size());
        if(m_strText[m_nCurrCharPos-1] == ('\n'))
        {
            m_strText.erase(m_nCurrCharPos - 1,1);
            m_nCurrCharPos--;
        }
        m_strText.erase(m_nCurrCharPos - 1, 1);
        m_nCurrCharPos--;
    }   
}

size_t CTextEdit::GetLineCount()const
{
    return m_uLineCount;
}

kmScalar CTextEdit::GetPadding() const
{
    return m_fPadding;
}

kmScalar CTextEdit::GetLineMaxHeight(size_t indexLine) const
{
   BEATS_ASSERT(indexLine < m_textOfLine.size())

   TString strText = m_textOfLine[indexLine];

   kmScalar fMaxHeight = 0.f;
   size_t size = strText.size();

   for( size_t i = 0; i < size; ++i)
   {
        CFontGlyph* pFontglyph = m_pFontFace->GetGlyph(strText[i]);
        if(pFontglyph && pFontglyph->height > fMaxHeight)
        {
            fMaxHeight = pFontglyph->height;
        }
   }
   return fMaxHeight;
}

const std::vector<TString>& CTextEdit::GetTextOfLines()
{
    CalcStringOfLine();
    return m_textOfLine;
}

void CTextEdit::SetMode(const EModeType &mode)
{
    m_mode = mode;
}

const CTextEdit::EModeType& CTextEdit::GetMode() const
{
    return m_mode;
}

void CTextEdit::PositionTextPos(const kmVec3 &localPos)
{
    if(!m_strText.empty())
    {
        kmScalar linewidth = 2 * m_fPadding;
        kmScalar rowheight = 2 * m_fPadding;
        kmScalar lastRowHeight = rowheight;
        size_t charIndex = 0;
        size_t rowIndex = 0;

        for(size_t i = 0; i < m_strText.size(); ++i)
        {
            if(m_strText[i] == '\n')
            {
                rowIndex++;
                linewidth = 2 * m_fPadding;
                lastRowHeight = rowheight;
            }
            else
            {
                CFontGlyph* pFontglyph = m_pFontFace->GetGlyph(m_strText[i]);
                BEATS_ASSERT(pFontglyph != nullptr);
                linewidth += pFontglyph->width;

                rowheight =  lastRowHeight + GetLineMaxHeight(rowIndex);
            }
            charIndex++;
            if(rowheight > localPos.y && linewidth > localPos.x)
                break;
        } 
        if(charIndex < m_strText.size())
        {
            m_bInsert = true;
        }
        else
        {
            m_bInsert = false;
        }
        m_nCurrCharPos = charIndex;
    } 
}

bool CTextEdit::OnMouseEvent( CMouseEvent *event )
{
    bool bRet = false;
    if( eMT_MODE_TEXTEDIT == m_mode && IsEnabled() )
    {
        if(event->Button() == GLFW_MOUSE_BUTTON_LEFT)
        {
            if(event->Type() == eEVT_EVENT_MOUSE_PRESSED)
            {
                kmVec3 pos;
                kmVec3Fill( &pos,event->X(), event->Y(), 0.f );
                CWindow::WorldToLocal( pos );

                if(IsContainPos(pos))
                {
                    PositionTextPos(pos);

                    m_bEdit = true;
                    WindowEvent event(eET_EVENT_TEXTEDIT_BEGINEDIT);
                    DispatchEvent(&event);
                }
                else
                {
                    m_bEdit = false;
                } 
            }
        }
        bRet = true;
    }
    return bRet;
}

bool CTextEdit::OnKeyboardEvent(CKeyboardEvent *event)
{
    bool bRet = false;
    if(IsEnabled() && m_bEdit)
    {
        if(event->Type() == eEVT_EVENT_CHAR)
        {
            TString strPreAdd ;
            strPreAdd += event->WChar();

            ProcessTextChange(strPreAdd);
        } 
        else if( event->Type() == eEVT_EVENT_KEY_PRESSED )
        {
            if( GLFW_KEY_ENTER == event->Key())
            {
                m_strText += _T("\n");
                m_uLineCount ++;
            }
            else if( GLFW_KEY_BACKSPACE == event->Key())
            {
                DeleteBackward();
            }
        }
        else if( event->Type() == eEVT_EVENT_KEY_REPEATED )
        {
            if( GLFW_KEY_BACKSPACE == event->Key())
            {
                DeleteBackward();
            }
        }
        bRet = true;
    }
    return bRet;
}

void CTextEdit::ProcessTextChange(TString strAdd)
{
    const char splitChar = _T('\n');
    std::string::size_type start = m_strText.find_first_not_of(splitChar);
    std::string::size_type  stop =m_strText.find_last_of(splitChar, start);
    TString result = m_strText.substr(stop);
    result += strAdd;
    kmScalar linewidth = 0;
    m_pFontFace->PrepareCharacters(result);
    for(size_t i = 0; i < result.size(); ++i)
    {
        CFontGlyph* pFontglyph = m_pFontFace->GetGlyph(result[i]);
        BEATS_ASSERT(pFontglyph);
        linewidth += pFontglyph->width;
    }

    linewidth += 2 * m_fPadding;
    if( linewidth  > Size().x )
    {
        if(IsPassedRowLimit())
        {
            WindowEvent event(eET_EVENT_TEXTEDIT_ENDEDIT);
            DispatchEvent(&event);

            m_bEdit = false;
        }
        else
        {
            WindowEvent event(eET_EVENT_TEXTEDIT_VALUECHANGE);
            DispatchEvent(&event);

            m_strText += _T("\n");
            AddText(strAdd);
            m_uLineCount++;
            m_nCurrCharPos++;
        }
    } 
}

bool CTextEdit::IsPassedRowLimit()
{   
    size_t size = m_textOfLine.size();
    kmScalar fToTalHeight = 0.f;
    kmScalar fLastHeight = 0.f;
    for(size_t i = 0; i < size; ++i)
    {
        kmScalar fMaxHeightOfLine = GetLineMaxHeight(i);
        fToTalHeight += fMaxHeightOfLine;
        if( i == size - 1)
            fLastHeight = fMaxHeightOfLine;
    }
    fToTalHeight += fLastHeight;
    fToTalHeight += 2 * m_fPadding;

    return fToTalHeight > Size().y;
}

bool CTextEdit::IsContainPos(const kmVec3 &pos)
{
    kmVec2 size = RealSize();
    return (pos.x >=0 && pos.x <= size.x 
        && pos.y >=0 && pos.y <= size.y );
}

void CTextEdit::CalcStringOfLine()
{
    m_textOfLine.clear();
    CStringHelper::GetInstance()->SplitString(m_strText.c_str(), _T("\n"), m_textOfLine);
}

void CTextEdit::AddText(TString strAdd)
{
    if( !m_bInsert )
    {
        m_strText += strAdd;
        m_nCurrCharPos++;
    }
    else
    {
        const TString string = m_strText.substr(m_nCurrCharPos);
        m_strText = m_strText.substr(0,m_nCurrCharPos);
        m_strText += strAdd;
        m_strText += string;

        std::string::size_type pos  = m_strText.find_first_of(_T('\n'), m_nCurrCharPos);
        while(pos !=  std::string::npos)
        {
            m_strText.erase(pos, 1);
            m_strText.insert(pos-1,_T("\n"));
             pos  = m_strText.find_first_of(_T('\n'), pos);
        }
    }
}

CFontFace* CTextEdit::GetFontFace() const
{
    return m_pFontFace;
}

