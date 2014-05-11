#ifndef GUI_FONT_FONT_H__INCLUDE
#define GUI_FONT_FONT_H__INCLUDE

#include "Resource/Resource.h"
#include "Resource/ResourcePublic.h"

namespace FCGUI
{
    class CFont : public CResource
    {
        DECLARE_RESOURCE_TYPE(eRT_Font);
    public:
        CFont();
        virtual ~CFont();

        virtual bool Load();

        virtual bool Unload();

        FT_Face Face() const;

    private:
        FT_Face m_face;

        static FT_Library m_library;
        static int m_nLibRefCount;
    };
}

#endif