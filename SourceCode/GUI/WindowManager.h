#ifndef GUI_WINDOW_MANAGER_H__INCLUDE
#define GUI_WINDOW_MANAGER_H__INCLUDE

#include "Window/WindowDefs.h"

//forward declaration
class CMouseEvent;
class CKeyboardEvent;

namespace FCGUI
{
    //forward declaration
    class CWindow;
    class CWindowFactory;
    class RendererFactory;

    class CWindowManager
    {
        BEATS_DECLARE_SINGLETON(CWindowManager);
        typedef std::map<TString, CWindow *> WindowList;

    public:
        void Init();
        CWindow *Create(const TString &name, EWindowType type, CWindow *parent);
        template <typename EWindowType>
        EWindowType *Create(const TString &name, CWindow *parent);
        void DestroyWindow(CWindow *window, bool delay = false);
        void DestroyWindow(const TString &name, bool delay = false);
        void OnWindowRename(CWindow *window, const TString &newname);
        void RegisterWindow(CWindow *window);
        void UnregisterWindow(CWindow *window);

        void Update(kmScalar dt);

        void registerWindowFactory(EWindowType windowType, CWindowFactory *windowFactory);
        void registerRendererFactory(EWindowType windowType, RendererFactory *renderFactory);

        CWindow *RootWindow() const;
        void SetFocusedWindow(CWindow *window);
        CWindow *FocusedWindow() const;
        CWindow *GetWindow(kmScalar x, kmScalar y) const;
        CWindow *GetWindow(const TString &name) const;
        size_t GetWindowCount() const;

        bool OnMouseEvent(CMouseEvent *event);
        bool OnKeyboardEvent(CKeyboardEvent *event);

    private:
        void registerDefaultWindowFactories();
        void registerDefaultRendererFactories();
        void destroyWindowImpl(CWindow *window);

    private:
        CWindow *m_pRootWindow;
        CWindow *m_pFocusedWindow;
        WindowList m_windowList;
        std::vector<CWindow *> m_recycleWindowList;
        std::map<EWindowType, RendererFactory *> m_rendererFactories;
        std::map<EWindowType, CWindowFactory *> m_windowFactories;
    };

    template <typename EWindowType>
    EWindowType *CWindowManager::Create(const TString &name, CWindow *parent)
    {
        return static_cast<EWindowType *>(Create(name, EWindowType::TYPE, parent));
    }
}

#endif