#ifndef GUI_WINDOW_LISTCONTROL_H__INCLUDE
#define GUI_WINDOW_LISTCONTROL_H__INCLUDE

#include "Window.h"

namespace FCGUI
{
    class CListControl : public CWindow
    {
        DECLARE_REFLECT_GUID(CListControl, 0x1C7228ED, CWindow)
        DEFINE_WINDOW_TYPE(eWT_WINDOW_LISTCONTROL);
    public:
        enum EventType
        {
            EVENT_LISTCONTROL = FCGUI::eGUIET_EVENT_LISTCONTROL,
            EVENT_MOVE,
        };
        CListControl();
        CListControl(const TString &name);
        virtual ~CListControl();

        virtual void ReflectData(CSerializer& serializer) override;
        virtual bool OnMouseEvent( CMouseEvent *event ) override;
        void AddItem(CWindow *item);
        void RemoveItem(CWindow *item);
        CWindow* GetItem(size_t row, size_t col);

        void SetRowHeight(kmScalar rowHeight);
        void SetColWidth(kmScalar colWidth);
    protected:
        void Init();
        void InitLayout();
        void CalcScrollOffsetRange();
        void CheckScrollOffset(kmVec2 &offset);
    private:
        bool m_bPressed;
        kmVec2 m_pressedPos;
        kmVec2 m_scrollOfffsetRange;
        kmVec2 m_scrollOffset;

        kmScalar m_fRowHeight;
        kmScalar m_fColWidth;
        kmScalar m_fTopMargin;
        kmScalar m_fBottomMargin;
        kmScalar m_fLeftMargin;
        kmScalar m_fRightMargin;
        kmScalar m_fHGap;
        kmScalar m_fVGap;
    };
}

#endif