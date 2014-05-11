#ifndef GUI_LAYOUT_BASE_LAYOUT_H__INCLUDE
#define GUI_LAYOUT_BASE_LAYOUT_H__INCLUDE

namespace FCGUI
{
    //forward declaration
    class CWindow;

    class CBaseLayout
    {
    public:
        CBaseLayout();
        virtual ~CBaseLayout();

        virtual void SetWindow(CWindow *window);

        bool Invalidated() const;

        virtual void PerformLayout() = 0;

    protected:
        void invalidate();

    protected:
        CWindow *m_pWindow;
        bool m_bInvalidated;
    };
}

#endif