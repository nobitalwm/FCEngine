#include "stdafx.h"
#include "WindowManager.h"
#include "Window/WindowFactory.h"
#include "Window/Window.h"
#include "Window/Button.h"
#include "Window/CheckBox.h"
#include "Window/Slider.h"
#include "Window/Progress.h"
#include "Window/TextEdit.h"
#include "Window/TextLabel.h"
#include "Window/ListBox.h"
#include "Window/ComboBox.h"
#include "Window/ListControl.h"
#include "Event/MouseEvent.h"
#include "Event/KeyboardEvent.h"
#include "Event/WindowEvent.h"
#include "Renderer/RendererFactory.h"
#include "Renderer/BaseRenderer.h"
#include "Renderer/CheckBoxRenderer.h"
#include "Renderer/SliderRenderer.h"
#include "Renderer/ProgressRenderer.h"
#include "Renderer/TextEditRenderer.h"
#include "Renderer/TextLabelRenderer.h"
#include "Renderer/ListBoxRenderer.h"
#include "Renderer/ComboBoxRenderer.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentManager.h"
using namespace FCGUI;

CWindowManager *CWindowManager::m_pInstance = nullptr;

CWindowManager::CWindowManager()
    : m_pFocusedWindow(nullptr)
    , m_pRootWindow(nullptr)
{
}

CWindowManager::~CWindowManager()
{
    for(auto window : m_windowList)
    {
        destroyWindowImpl(window.second);
    }
    for(auto windowFactory : m_windowFactories)
    {
        BEATS_SAFE_DELETE(windowFactory.second);
    }
    for(auto rendererFactory : m_rendererFactories)
    {
        BEATS_SAFE_DELETE(rendererFactory.second);
    }
}

void CWindowManager::Init()
{
    registerDefaultWindowFactories();
    registerDefaultRendererFactories();
    m_pRootWindow = Create(_T("Root"), eWT_WINDOW_BASE, nullptr);
}

CWindow *CWindowManager::Create(const TString &name, EWindowType type, CWindow *parent)
{
    BEATS_ASSERT(m_windowList.find(name) == m_windowList.end());

    CWindow *window = nullptr;
    auto itr = m_windowFactories.find(type);
    BEATS_ASSERT(itr != m_windowFactories.end());
    if(itr != m_windowFactories.end())
    {
        window = itr->second->Create(name);

        auto itr = m_rendererFactories.find(window->Type());
        if(itr != m_rendererFactories.end())
        {
            window->SetRenderer(itr->second->Create());
        }

        if(!parent && m_pRootWindow)
        {
            parent = m_pRootWindow;
        }
        if(parent)
            parent->AddChild(window);

        WindowEvent event(CWindow::eET_EVENT_INITIALIZED);
        window->DispatchEvent(&event);
    }

    return window;
}

void CWindowManager::DestroyWindow(CWindow *window, bool delay)
{
    BEATS_ASSERT(window->Parent());

    DestroyWindow(window->Name(), delay);
}

void CWindowManager::DestroyWindow(const TString &name, bool delay)
{
    auto itr = m_windowList.find(name);
    BEATS_ASSERT(itr != m_windowList.end());
    CWindow *window = itr->second;
    m_windowList.erase(itr);
    window->Parent()->RemoveChild(window);

    if(window == FocusedWindow())
        SetFocusedWindow(nullptr);

    WindowEvent event(CWindow::eET_EVENT_DESTROY);
    window->DispatchEvent(&event);

    if(delay)
    {
        m_recycleWindowList.push_back(window);
    }
    else
    {
        destroyWindowImpl(window);
    }
}

void CWindowManager::destroyWindowImpl(CWindow *window)
{
    if(!CComponentManager::GetInstance()->GetComponentInstance(window->GetId(), window->GetGuid()) )
    {
        CComponentBase *temp = CComponentManager::GetInstance()->GetComponentTemplate(window->GetGuid());
        if(temp != window)
        {
            BEATS_SAFE_DELETE(window);
        }
    }
}

void CWindowManager::OnWindowRename(CWindow *window, const TString &newname)
{
    auto itr = m_windowList.find(window->Name());
    if(itr != m_windowList.end())
    {
        BEATS_ASSERT(itr->second == window);
        m_windowList.erase(itr);
    }

    if(!newname.empty())
    {
        m_windowList[newname] = window;
    }

    if(window->Name().empty() && !newname.empty())
    {
        WindowEvent event(CWindow::eET_EVENT_REGISTERED);
        window->DispatchEvent(&event);
    }
    else if(!window->Name().empty() && newname.empty())
    {
        WindowEvent event(CWindow::eET_EVENT_UNREGISTERED);
        window->DispatchEvent(&event);
    }
}

void CWindowManager::RegisterWindow(CWindow *window)
{
    BEATS_ASSERT(!window->Name().empty());
    BEATS_ASSERT(m_windowList.find(window->Name()) == m_windowList.end());
    m_windowList[window->Name()] = window;

    WindowEvent event(CWindow::eET_EVENT_REGISTERED);
    window->DispatchEvent(&event);
}

void CWindowManager::UnregisterWindow(CWindow *window)
{
    auto itr = m_windowList.find(window->Name());
    if(itr != m_windowList.end())
    {
        BEATS_ASSERT(itr->second == window);
        m_windowList.erase(itr);

        if(window == FocusedWindow())
            SetFocusedWindow(nullptr);

        WindowEvent event(CWindow::eET_EVENT_UNREGISTERED);
        window->DispatchEvent(&event);
    }
}

CWindow *CWindowManager::RootWindow() const
{
    return m_pRootWindow;
}

void CWindowManager::SetFocusedWindow( CWindow *window )
{
    m_pFocusedWindow = window;
}

CWindow *CWindowManager::FocusedWindow() const
{
    return m_pFocusedWindow;
}

CWindow *CWindowManager::GetWindow(kmScalar x, kmScalar y) const
{
    BEATS_ASSERT(m_pRootWindow);
    CWindow *result = m_pRootWindow->GetChild(x, y);
    return result;
}

CWindow *CWindowManager::GetWindow(const TString &name) const
{
    auto itr = m_windowList.find(name);
    return itr != m_windowList.end() ? itr->second : nullptr;
}

size_t CWindowManager::GetWindowCount() const
{
    return m_windowList.size();
}

void CWindowManager::Update(kmScalar dt)
{
    for(auto window : m_recycleWindowList)
    {
        destroyWindowImpl(window);
    }
    m_recycleWindowList.clear();

    m_pRootWindow->Update(dt);
}

bool CWindowManager::OnMouseEvent( CMouseEvent *event )
{
    if(m_pFocusedWindow)
        m_pFocusedWindow->OnMouseEvent(event);

    if(!event->Stopped())
    {
        if(event->Type() == eEVT_EVENT_MOUSE_PRESSED)
        {
            m_pRootWindow->OnMouseEvent(event);
        }
    }
    return true;
}

bool CWindowManager::OnKeyboardEvent(CKeyboardEvent *event)
{
    if(m_pFocusedWindow)
        m_pFocusedWindow->OnKeyboardEvent(event);
    return true;
}

void CWindowManager::registerWindowFactory( EWindowType windowType, CWindowFactory *windowFactory )
{
    auto itr = m_windowFactories.find(windowType);
    if(itr != m_windowFactories.end())
    {
        BEATS_SAFE_DELETE(itr->second);
        itr->second = windowFactory;
    }
    m_windowFactories[windowType] = windowFactory;
}

void CWindowManager::registerRendererFactory( EWindowType windowType, RendererFactory *renderFactory )
{
    auto itr = m_rendererFactories.find(windowType);
    if(itr != m_rendererFactories.end())
    {
        BEATS_SAFE_DELETE(itr->second);
        itr->second = renderFactory;
    }
    m_rendererFactories[windowType] = renderFactory;
}

void CWindowManager::registerDefaultWindowFactories()
{
    registerWindowFactory(eWT_WINDOW_BASE, new TplWindowFactory<CWindow>());
    registerWindowFactory(eWT_WINDOW_BUTTON, new TplWindowFactory<CButton>());
    registerWindowFactory(eWT_WINDOW_CHECKBOX, new TplWindowFactory<CCheckBox>());
    registerWindowFactory(eWT_WINDOW_SLIDER, new TplWindowFactory<CSlider>());
    registerWindowFactory(eWT_WINDOW_PROGRESS, new TplWindowFactory<CProgress>());
    registerWindowFactory(eWT_WINDOW_TEXTEDIT, new TplWindowFactory<CTextEdit>());
    registerWindowFactory(eWT_WINDOW_TEXTLABEL, new TplWindowFactory<CTextLabel>());
    registerWindowFactory(eWT_WINDOW_LISTBOX, new TplWindowFactory<CListBox>());
    registerWindowFactory(eWT_WINDOW_COMBOBOX, new TplWindowFactory<CComboBox>());
    registerWindowFactory(eWT_WINDOW_LISTCONTROL, new TplWindowFactory<CListControl>());
}

void CWindowManager::registerDefaultRendererFactories()
{
    registerRendererFactory(eWT_WINDOW_BASE, new TplRendererFactory<CBaseRenderer>());
    registerRendererFactory(eWT_WINDOW_BUTTON, new TplRendererFactory<CBaseRenderer>());
    registerRendererFactory(eWT_WINDOW_CHECKBOX, new TplRendererFactory<CCheckBoxRenderer>());
    registerRendererFactory(eWT_WINDOW_SLIDER, new TplRendererFactory<CSliderRenderer>());
    registerRendererFactory(eWT_WINDOW_PROGRESS, new TplRendererFactory<CProgressRenderer>());
    registerRendererFactory(eWT_WINDOW_TEXTEDIT, new TplRendererFactory<CTextEditRenderer>());
    registerRendererFactory(eWT_WINDOW_TEXTLABEL, new TplRendererFactory<CTextLabelRenderer>());
    registerRendererFactory(eWT_WINDOW_LISTBOX, new TplRendererFactory<CListBoxRenderer>());
    registerRendererFactory(eWT_WINDOW_COMBOBOX, new TplRendererFactory<CComboBoxRenderer>());
    registerRendererFactory(eWT_WINDOW_LISTCONTROL, new TplRendererFactory<CBaseRenderer>());
}

