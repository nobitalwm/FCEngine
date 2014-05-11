#include "stdafx.h"
#include "Font.h"
#include "Utility/BeatsUtility/StringHelper.h"

using namespace FCGUI;

FT_Library CFont::m_library = nullptr;
int CFont::m_nLibRefCount = 0;

CFont::CFont()
    : m_face(nullptr)
{
    if(m_nLibRefCount++ == 0)
    {
        FT_Error err = FT_Init_FreeType(&m_library);
        err;
        BEATS_ASSERT(!err);
    }
}

CFont::~CFont()
{
    if(--m_nLibRefCount == 0)
    {
        FT_Error err = FT_Done_FreeType(m_library);
        err;
        BEATS_ASSERT(!err);
    }
}

bool CFont::Load()
{
    BEATS_ASSERT(!IsLoaded(), _T("Can't Load a font which is already loaded!"));

    size_t len = m_strPath.size() * 2 + 1;
    char *path = new char[len];
    CStringHelper::GetInstance()->ConvertToCHAR(m_strPath.c_str(), path, len);

    FT_Error err = FT_New_Face(m_library, path, 0, &m_face);
    BEATS_ASSERT(!err);

    err = FT_Select_Charmap(m_face, FT_ENCODING_UNICODE);
    BEATS_ASSERT(!err);

    BEATS_SAFE_DELETE_ARRAY(path);

    SetLoadedFlag(!err);

    return !err;
}

bool CFont::Unload()
{
    BEATS_ASSERT(IsLoaded(), _T("Can't Unload a font which haven't been loaded!"));

    FT_Error err = FT_Done_Face(m_face);
    BEATS_ASSERT(!err);

    SetLoadedFlag(false);

    return !err;
}

FT_Face CFont::Face() const
{
    return m_face;
}
