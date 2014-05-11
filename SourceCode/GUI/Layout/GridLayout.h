#ifndef GUI_LAYOUT_GRID_LAYOUT_H__INCLUDE
#define GUI_LAYOUT_GRID_LAYOUT_H__INCLUDE

#include "BaseLayout.h"

namespace FCGUI
{
    //forward declaration
    class CWindow;

    class CGridLayout : public CBaseLayout
    {
    public:
        CGridLayout(size_t rowCount, size_t colCount);
        virtual ~CGridLayout();

        virtual void SetWindow(CWindow *window) override;

        void SetRowCount(size_t rowCount);
        size_t RowCount() const;
        void SetColCount(size_t colCount);
        size_t ColCount() const;

        void SetMargin(kmScalar topMargin, kmScalar bottomMargin, 
            kmScalar leftMargin, kmScalar rightMargin);
        kmScalar TopMargin() const;
        kmScalar BottomMargin() const;
        kmScalar LeftMargin() const;
        kmScalar RightMargin() const;
        void SetGap(kmScalar hGap, kmScalar vGap);
        kmScalar HGap() const;
        kmScalar VGap() const;

        void SetRowHeight(kmScalar rowHeight);
        void SetColWidth(kmScalar colWidth);
        void SetRowHeightSingleFixed(size_t row, kmScalar height);
        void SetRowHeightSingleFactored(size_t row, kmScalar heightFactor);
        void SetColWidthSingleFixed(size_t col, kmScalar width);
        void SetColWidthSingleFactored(size_t col, kmScalar widthFactor);

        virtual bool AddChild(CWindow *window);
        virtual bool AddChild(CWindow *window, kmScalar x, kmScalar y);
        virtual bool AddChildToCell(CWindow *window, size_t row, size_t col);
        virtual bool RemoveChild(CWindow *window);
        CWindow *GetChild(size_t row, size_t col);

        virtual void PerformLayout() override;

    protected:
        void CalcSplitPos();

    protected:
        size_t m_uRowCount;
        size_t m_uColCount;
        kmScalar m_fTopMargin;
        kmScalar m_fBottomMargin;
        kmScalar m_fLeftMargin;
        kmScalar m_fRightMargin;
        kmScalar m_fHGap;
        kmScalar m_fVGap;
        std::vector<std::pair<kmScalar, kmScalar> > m_everyRowHeight;    //first:fixed second:factor
        kmScalar m_fRowHeight;
        std::vector<std::pair<kmScalar, kmScalar> > m_everyColWidth;     //first:fixed second:factor
        kmScalar m_fColWidth;
        std::vector<std::pair<kmScalar, kmScalar> > m_hSplitPos;
        std::vector<std::pair<kmScalar, kmScalar> > m_vSplitPos;

        std::vector<CWindow *> m_children;
    };
}

#endif