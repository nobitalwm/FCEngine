#ifndef GUI_LAYOUT_VSCROLL_LAYOUT_H__INCLUDE
#define GUI_LAYOUT_VSCROLL_LAYOUT_H__INCLUDE

#include "ScrollLayout.h"

namespace FCGUI
{
    class CVScrollLayout : public CScrollLayout
    {
    public:
        CVScrollLayout(size_t colCount = 1);
    };
}

#endif