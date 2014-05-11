#ifndef GUI_LAYOUT_SCROLL_LAYOUT_H__INCLUDE
#define GUI_LAYOUT_SCROLL_LAYOUT_H__INCLUDE

#include "GridLayout.h"

namespace FCGUI
{
    class CScrollLayout : public CGridLayout
    {
    public:
        CScrollLayout(size_t rowCount, size_t colCount);

        virtual bool AddChild( CWindow *window ) override;

        virtual bool AddChildToCell( CWindow *window, size_t row, size_t col ) override;

        virtual bool RemoveChild( CWindow *window );

    protected:
        kmScalar m_fColWidth;
        kmScalar m_fRowHeight;
    };
}

#endif