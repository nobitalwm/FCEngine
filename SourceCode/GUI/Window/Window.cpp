#include "stdafx.h"
#include "Window.h"
#include "GUI/System.h"
#include "GUI/Event/WindowEvent.h"
#include "GUI/Layout/BaseLayout.h"
#include "GUI/Behavior/BehaviorBase.h"
#include "GUI/Renderer/BaseRenderer.h"
#include "GUI/WindowManager.h"
#include "Event/MouseEvent.h"
#include "Event/KeyboardEvent.h"
#include "GUI/Animation/AnimationInstance.h"
#include "GUI/Animation/AnimationManager_ui.h"

#define VAR_NAME(var) _T(#var)

using namespace FCGUI;

CWindow::CWindow()
{
    Init();
}

CWindow::CWindow(const TString &name)
{
    Init(name);
}

CWindow::~CWindow()
{
    BEATS_SAFE_DELETE(m_pRenderer);
    BEATS_SAFE_DELETE(m_pLayout);
}

void CWindow::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_strName, true, 0xFFFFFFFF, _T("名称"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_pos, true, 0xFFFFFFFF, _T("位置"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_percentPos, true, 0xFFFFFFFF, _T("位置百分比"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_size, true, 0xFFFFFFFF, _T("尺寸"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_percentSize, true, 0xFFFFFFFF, _T("尺寸百分比"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_fRotation, true, 0xFFFFFFFF, _T("旋转"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_scale, true, 0xFFFFFFFF, _T("缩放"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_bVisible, true, 0xFFFFFFFF, _T("可见"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_anchor, true, 0xFFFFFFFF, _T("锚点"), NULL, NULL, NULL);
}

void CWindow::OnPropertyChanged(const TString &variableName, void *value)
{
    if(variableName == VAR_NAME(m_strName))
    {
        SetName(*(TString *)value);
    }
    else if(variableName == VAR_NAME(m_pos))
    {
        SetPos(*(kmVec2 *)value);
    }
    else if(variableName == VAR_NAME(m_percentPos))
    {
        SetPercentPos(*(kmVec2 *)value);
    }
    else if(variableName == VAR_NAME(m_size))
    {
        SetSize(*(kmVec2 *)value);
    }
    else if(variableName == VAR_NAME(m_percentSize))
    {
        SetPercentSize(*(kmVec2 *)value);
    }
    else if(variableName == VAR_NAME(m_fRotation))
    {
        SetRotation(*(kmScalar *)value);
    }
    else if(variableName == VAR_NAME(m_scale))
    {
        SetScale(*(kmVec2 *)value);
    }
    else if(variableName == VAR_NAME(m_bVisible))
    {
        SetVisible(*(bool *)value);
    }
    else if(variableName == VAR_NAME(m_anchor))
    {
        SetAnchor(*(kmVec2 *)value);
    }
}

void CWindow::Init(const TString &name)
{
    AddProp(ePT_PROP_X, _T("X"), 
        [this](kmScalar x){SetPos(x, Pos().y);}, [this](){return Pos().x;});
    AddProp(ePT_PROP_Y, _T("Y"), 
        [this](kmScalar y){SetPos(Pos().x, y);}, [this](){return Pos().y;});
    AddProp(ePT_PROP_PERCENT_X, _T("X%"), 
        [this](kmScalar xPer){SetPercentPos(xPer, PercentPos().y);}, [this](){return PercentPos().x;});
    AddProp(ePT_PROP_PERCENT_Y, _T("Y%"), 
        [this](kmScalar yPer){SetPercentPos(PercentPos().x, yPer);}, [this](){return PercentPos().y;});
    AddProp(ePT_PROP_WIDTH, _T("Width"), 
        [this](kmScalar width){SetSize(width, Size().y);}, [this](){return Size().x;});
    AddProp(ePT_PROP_HEIGHT, _T("Height"), 
        [this](kmScalar height){SetSize(Size().x, height);}, [this](){return Size().y;});
    AddProp(ePT_PROP_PERCENT_WIDTH, _T("Width%"), 
        [this](kmScalar wPer){SetPercentSize(wPer, PercentSize().y);}, [this](){return PercentSize().x;});
    AddProp(ePT_PROP_PERCENT_HEIGHT, _T("Height%"), 
        [this](kmScalar hPer){SetPercentSize(PercentSize().x, hPer);}, [this](){return PercentSize().y;});
    AddProp(ePT_PROP_ANCHOR_X, _T("Anchor X"), 
        [this](kmScalar x){SetAnchor(x, Anchor().y);}, [this](){return Anchor().x;});
    AddProp(ePT_PROP_ANCHOR_Y, _T("Anchor Y"), 
        [this](kmScalar y){SetAnchor(Anchor().x, y);}, [this](){return Anchor().y;});
    AddProp(ePT_PROP_ROTATION, _T("Rotation"), 
        [this](kmScalar r){SetRotation(r);}, [this](){return Rotation();});
    AddProp(ePT_PROP_SCALE_X, _T("Scale X"), 
        [this](kmScalar x){SetScale(x, Scale().y);}, [this](){return Scale().x;});
    AddProp(ePT_PROP_SCALE_Y, _T("Scale Y"), 
        [this](kmScalar y){SetScale(Scale().x, y);}, [this](){return Scale().y;});

    m_pParent = nullptr;
    m_pRenderer = nullptr;
    m_pLayout = nullptr;

    kmVec2Fill(&m_pos, 0.f, 0.f);
    kmVec2Fill(&m_percentPos, 0.f, 0.f);
    kmVec2Fill(&m_realPos, 0.f, 0.f);
    kmVec2Fill(&m_size, 0.f, 0.f);
    kmVec2Fill(&m_percentSize, 0.f, 0.f);
    kmVec2Fill(&m_realSize, 0.f, 0.f);
    kmVec2Fill(&m_anchor, 0.f, 0.f);
    m_fRotation = 0.f;
    kmVec2Fill(&m_scale, 1.f, 1.f);
    kmVec2Fill(&m_scrolloffset, 0.f, 0.f);
    m_bVisible = true;
    m_bEnabled = true;
    m_bClipping = false;

    if(name.empty())
    {
        TCHAR autoname[32];
        _stprintf_s(autoname, _T("Window%p"), this);
        SetName(autoname);
    }
    else
    {
        SetName(name);
    }
}

void CWindow::SetPosSize(kmScalar x, kmScalar y, kmScalar xPercent, kmScalar yPercent, 
    kmScalar width, kmScalar height, kmScalar widthPercent, kmScalar heightPercent)
{
    SetPos(x, y);
    SetPercentPos(xPercent, yPercent);
    SetSize(width, height);
    SetPercentSize(widthPercent, heightPercent);
}

void CWindow::SetArea(kmScalar left, kmScalar top, kmScalar leftPercent, kmScalar topPercent, 
    kmScalar right, kmScalar bottom, kmScalar rightPercent, kmScalar bottomPercent)
{
    SetPos(left, top);
    SetPercentPos(leftPercent, topPercent);
    SetSize(right - left, bottom - top);
    SetPercentSize(rightPercent - leftPercent, bottomPercent - topPercent);
}

void CWindow::SetRenderer(CBaseRenderer *renderer)
{
    m_pRenderer = renderer;
    renderer->SetWindow(this);
}

CBaseRenderer *CWindow::Renderer() const
{
    return m_pRenderer;
}

void CWindow::SetLayout(CBaseLayout *layout)
{
    m_pLayout = layout;
    layout->SetWindow(this);
}

CBaseLayout *CWindow::Layout() const
{
    return m_pLayout;
}

void CWindow::SetName(const TString &name)
{
    if(name != m_strName)
    {
        CWindowManager::GetInstance()->OnWindowRename(this, name);
        m_strName = name;
    }
}

TString CWindow::Name() const
{
    return m_strName;
}

void CWindow::SetPos( kmScalar x, kmScalar y )
{
    m_pos.x = x;
    m_pos.y = y;
    CalcRealPos();
}

void CWindow::SetPos( kmVec2 pos )
{
    SetPos(pos.x, pos.y);
}

kmVec2 CWindow::Pos() const
{
    return m_pos;
}

void CWindow::SetPercentPos( kmScalar x, kmScalar y )
{
    m_percentPos.x = x;
    m_percentPos.y = y;
    CalcRealPos();
}

void CWindow::SetPercentPos( kmVec2 posPercent )
{
    SetPercentPos(posPercent.x, posPercent.y);
}

kmVec2 CWindow::PercentPos() const
{
    return m_percentPos;
}

void CWindow::CalcRealPos()
{
    if(!m_pParent)
    {
        m_realPos = m_pos;
    }
    else
    {
        kmVec2 parentSize = m_pParent->RealSize();
        kmVec2 parentAnchor;
        parentAnchor.x = parentSize.x * m_pParent->Anchor().x;
        parentAnchor.y = parentSize.y * m_pParent->Anchor().y;
        m_realPos.x = parentSize.x * m_percentPos.x + m_pos.x - parentAnchor.x;
        m_realPos.y = parentSize.y * m_percentPos.y + m_pos.y - parentAnchor.y;
    }

    CalcTransform();

    WindowEvent event(eET_EVENT_MOVED);
    DispatchEvent(&event);
}

kmVec2 CWindow::RealPos() const
{
    return m_realPos;
}

void CWindow::SetSize( kmScalar width, kmScalar height )
{
    m_size.x = width;
    m_size.y = height;
    CalcRealSize();
}

void CWindow::SetSize( kmVec2 size )
{
    SetSize(size.x, size.y);
}

kmVec2 CWindow::Size() const
{
    return m_size;
}

void CWindow::SetPercentSize( kmScalar width, kmScalar height )
{
    m_percentSize.x = width;
    m_percentSize.y = height;
    CalcRealSize();
}

void CWindow::SetPercentSize( kmVec2 sizePercent )
{
    SetPercentSize(sizePercent.x, sizePercent.y);
}

kmVec2 CWindow::PercentSize() const
{
    return m_percentSize;
}

void CWindow::CalcRealSize()
{
    if(!m_pParent)
    {
        m_realSize = m_size;
    }
    else
    {
        kmVec2 parentSize = m_pParent->RealSize();
        m_realSize.x = parentSize.x * m_percentSize.x + m_size.x;
        m_realSize.y = parentSize.y * m_percentSize.y + m_size.y;
    }

    WindowEvent event(eET_EVENT_SIZED);
    DispatchEvent(&event);

    for(auto child : m_children)
    {
        child->OnParentSized();
    }
}

void CWindow::OnScrollOffset()
  {
      for(auto child : m_children)
      {
          child->CalcChildRealPos();
      }
  }

void CWindow::CalcChildRealPos()
{
    if(!m_pParent)
    {
        m_realPos = m_pos;
    }
    else
    {
        kmVec2 parentSize = m_pParent->RealSize();
        kmVec2 parentAnchor;
        parentAnchor.x = parentSize.x * m_pParent->Anchor().x;
        parentAnchor.y = parentSize.y * m_pParent->Anchor().y;
        m_realPos.x = parentSize.x * m_percentPos.x + m_pos.x - parentAnchor.x - m_pParent->m_scrolloffset.x;
        m_realPos.y = parentSize.y * m_percentPos.y + m_pos.y - parentAnchor.y - m_pParent->m_scrolloffset.y;
    }

    CalcTransform();

    WindowEvent event(eET_EVENT_MOVED);
    DispatchEvent(&event);
}

kmVec2 CWindow::RealSize() const
{
    return m_realSize;
}

void CWindow::SetAnchor( kmScalar x, kmScalar y )
{
    m_anchor.x = x;
    m_anchor.y = y;

    WindowEvent event(eET_EVENT_ANCHOR_CHANGED);
    DispatchEvent(&event);
}

void CWindow::SetAnchor( kmVec2 anchor )
{
    SetAnchor(anchor.x, anchor.y);
}

kmVec2 CWindow::Anchor() const
{
    return m_anchor;
}

kmVec2 CWindow::RealAnchor() const
{
    kmVec2 realAnchor;
    kmVec2Fill(&realAnchor, m_realSize.x * m_anchor.x, m_realSize.y * m_anchor.y);
    return realAnchor;
}

void CWindow::SetRotation( kmScalar rotation )
{
    m_fRotation = rotation;

    CalcTransform();

    WindowEvent event(eET_EVENT_ROTATED);
    DispatchEvent(&event);
}

kmScalar CWindow::Rotation() const
{
    return m_fRotation;
}

bool CWindow::IsRotated() const
{
    return !BEATS_FLOAT_EQUAL(m_fRotation, 0.f) || (m_pParent && m_pParent->IsRotated());
}

void CWindow::SetScale( kmScalar scaleX, kmScalar scaleY )
{
    m_scale.x = scaleX;
    m_scale.y = scaleY;

    CalcTransform();

    WindowEvent event(eET_EVENT_SCALED);
    DispatchEvent(&event);
}

void CWindow::SetScale( kmVec2 scale )
{
    SetScale(scale.x, scale.y);
}

kmVec2 CWindow::Scale() const
{
    return m_scale;
}

void CWindow::Scroll(kmVec2 offset)
{
    kmVec2Add(&m_scrolloffset, &m_scrolloffset, &offset);
    OnScrollOffset();
}

void CWindow::CalcTransform()
{
    kmMat4 scale;
    kmMat4Identity(&scale);
    kmMat4Scaling(&scale, m_scale.x, m_scale.y, 1.f);
    kmMat4 rot;
    kmMat4Identity(&rot);
    kmMat4RotationZ(&rot, kmDegreesToRadians(m_fRotation));
    kmMat4 trans;
    kmMat4Identity(&trans);
    kmMat4Translation(&trans, m_realPos.x, m_realPos.y, 0.f);
    kmMat4Multiply(&m_transform, &rot, &scale);
    kmMat4Multiply(&m_transform, &trans, &m_transform);
}

const kmMat4 &CWindow::Transform() const
{
    return m_transform;
}

kmMat4 CWindow::WorldTransform() const
{
    if(!m_pParent)
    {
        return m_transform;
    }
    else
    {
        kmMat4 worldMat;
        kmMat4 parentWorldMat = m_pParent->WorldTransform();
        kmMat4Multiply(&worldMat, &m_transform, &parentWorldMat);
        return worldMat;
    }
}

void CWindow::WorldToLocal(kmVec3 &pos) const
{
    if(m_pParent)
    {
        m_pParent->WorldToLocal(pos);
    }

    ParentToLocal(pos);
}

void CWindow::WorldToLocal(kmScalar &x, kmScalar &y) const
{
    kmVec3 pos;
    kmVec3Fill(&pos, x, y, 0.f);
    WorldToLocal(pos);
    x = pos.x;
    y = pos.y;
}

void CWindow::ParentToLocal(kmVec3 &pos) const
{
    kmMat4 inverseMat;
    kmMat4Inverse(&inverseMat, &m_transform);
    kmVec3Transform(&pos, &pos, &inverseMat);
}

void CWindow::ParentToLocal(kmScalar &x, kmScalar &y) const
{
    kmVec3 pos;
    kmVec3Fill(&pos, x, y, 0.f);
    ParentToLocal(pos);
    x = pos.x;
    y = pos.y;
}

void CWindow::LocalToWorld(kmVec3 &pos) const
{
    LocalToParent(pos);

    if(m_pParent)
    {
        m_pParent->LocalToWorld(pos);
    }
}

void CWindow::LocalToWorld(kmScalar &x, kmScalar &y) const
{
    kmVec3 pos;
    kmVec3Fill(&pos, x, y, 0.f);
    LocalToWorld(pos);
    x = pos.x;
    y = pos.y;
}

void CWindow::LocalToParent(kmVec3 &pos) const
{
    kmVec3Transform(&pos, &pos, &m_transform);
}

void CWindow::LocalToParent(kmScalar &x, kmScalar &y) const
{
    kmVec3 pos;
    kmVec3Fill(&pos, x, y, 0.f);
    LocalToParent(pos);
    x = pos.x;
    y = pos.y;
}

CWindow::EHitTestResult CWindow::HitTest(kmScalar x, kmScalar y, bool localized) const
{
    static const kmScalar DEVIATION = 5.f;

    kmVec2 realAnchor = RealAnchor();

    if(!localized)
        WorldToLocal(x, y);

    kmScalar toLeft = x - (-realAnchor.x);
    kmScalar toRight = (-realAnchor.x + m_realSize.x) - x;
    kmScalar toTop = y - (-realAnchor.y);
    kmScalar toBottom = (-realAnchor.y + m_realSize.y) - y;

    EHitTestResult result = eHTR_HIT_NONE;
    if(toLeft >= 0.f && toRight >= 0.f && toTop >= 0.f && toBottom >= 0.f)
    {
        result = eHTR_HIT_CONTENT;
        if(toLeft <= DEVIATION)
        {
            if(toTop <= DEVIATION)
            {
                result = eHTR_HIT_TOP_LEFT;
            }
            else if(toBottom <= DEVIATION)
            {
                result = eHTR_HIT_BOTTOM_LEFT;
            }
        }
        else if(toRight <= DEVIATION)
        {
            if(toTop <= DEVIATION)
            {
                result = eHTR_HIT_TOP_RIGHT;
            }
            else if(toBottom <= DEVIATION)
            {
                result = eHTR_HIT_BOTTOM_RIGHT;
            }
            else if(abs(toBottom - toTop) <= DEVIATION*2)
            {
                result = eHTR_HIT_ROTATION_HANDLE;
            }
        }
        else if(abs(toLeft - realAnchor.x) <= DEVIATION*2 
            &&  abs(toTop - realAnchor.y) <= DEVIATION*2)
        {
            result = eHTR_HIT_ANCHOR;
        }
    }

    return result;
}

void CWindow::Show()
{
    SetVisible(true);
}

void CWindow::Hide()
{
    SetVisible(false);
}

void CWindow::ToggleVisible()
{
    SetVisible(!IsVisible());
}

void CWindow::SetVisible(bool visible)
{
    if(visible != m_bVisible)
    {
        m_bVisible = visible;
        WindowEvent event(eET_EVENT_VISIBLE_CHANGED);
        DispatchEvent(&event);
    }
}

bool CWindow::IsVisible() const
{
    return m_bVisible;
}

void CWindow::Enable()
{
    m_bEnabled = true;
}

void CWindow::Disable()
{
    m_bEnabled = false;
}

bool CWindow::IsEnabled() const
{
    return m_bEnabled && (!m_pParent || m_pParent->IsEnabled());
}

void CWindow::SetClipping(bool clipping)
{
    m_bClipping = clipping;
}

bool CWindow::IsClipping() const
{
    return (m_bClipping && !IsRotated()) || (m_pParent && m_pParent->IsClipping());
}

void CWindow::GetScissorRect(kmVec2 &pos, kmVec2 &size)
{
    if(m_bClipping)
    {
        kmVec2 anchor = RealAnchor();
        kmVec2Fill(&pos, -anchor.x, -anchor.y);
        LocalToWorld(pos.x, pos.y);
        kmVec2 bottomRight = RealSize();
        bottomRight.x -= anchor.x;
        bottomRight.y -= anchor.y;
        LocalToWorld(bottomRight.x, bottomRight.y);
        size.x = bottomRight.x - pos.x;
        size.y = bottomRight.y - pos.y;
        //Y axis is bottom-up in OpenGL
        pos.y = CSystem::GetInstance()->GetResolution().y - pos.y - size.y;
    }
    else if(m_pParent)
    {
        m_pParent->GetScissorRect(pos, size);
    }
}

int CWindow::CurrState() const
{
    return m_bEnabled ? eST_STATE_NORMAL : eST_STATE_DISABLED;
}

void CWindow::OnParentSized()
{
    CalcRealPos();
    CalcRealSize();
}

bool CWindow::OnMouseEvent( CMouseEvent *event )
{
    bool handled = false;

    bool hitted = HitTest(event->X(), event->Y()) > eHTR_HIT_NONE;
    if(hitted || !m_bClipping)
    {
        for(size_t i = 0; i < m_children.size(); ++i)
        {
            CWindow *child = m_children[i];
            if(child->OnMouseEvent(event))
            {
                handled = true;
                if(event->Stopped())
                    break;
            }
        }
    }

    if(hitted && !event->Stopped())
    {
        DispatchEvent(event);

        if(event->Type() == eEVT_EVENT_MOUSE_PRESSED)
        {
            CWindowManager::GetInstance()->SetFocusedWindow(this);
            event->StopPropagation();
        }

        handled = true;
    }
    return handled;
}

bool CWindow::OnKeyboardEvent(CKeyboardEvent *event)
{
    for(auto child : m_children)
    {
        child->OnKeyboardEvent(event);
        if(event->Stopped())
        {
            return true;
        }
    }

    if(!event->Stopped())
    {
        DispatchEvent(event);
    }

    return true;
}

void CWindow::Update(float deltaTime)
{
    //perform layout if nesessary
    if(m_pLayout && m_pLayout->Invalidated())
    {
        m_pLayout->PerformLayout();
    }
    
    //update children
    for(auto child : m_children)
    {
        child->Update(deltaTime);
    }
}

void CWindow::DispatchEvent(CBaseEvent *event)
{
    CEventDispatcher::DispatchEvent(event);

    auto itr = m_autoAnimations.find(event->Type());
    if(itr != m_autoAnimations.end())
    {
        CAnimationInstance *instance = 
            CAnimationManager::GetInstance()->InstantiateAnimation(itr->second, this, 
            CAnimation::EReplayMode::eRM_DEFAULT, 1.f, true);
        instance->Play();
    }
}

void CWindow::AddAutoAnimation(int eventType, const TString &animName)
{
    m_autoAnimations[eventType] = animName;
}

void CWindow::SetParent( CWindow *parent )
{
    if(parent != m_pParent)
    {
        if(m_pParent)
        {
            m_pParent->RemoveChild(this);
        }
        m_pParent = parent;
        CalcRealPos();
        CalcRealSize();
    }
}

CWindow *CWindow::Parent() const
{
    return m_pParent;
}

void CWindow::AddChild( CWindow *window )
{
    BEATS_ASSERT(window);
    BEATS_ASSERT(!GetChild(window->Name()), _T("You can't add a window twice."));

    m_children.push_back(window);
    window->SetParent(this);

    WindowEvent event(EVENT_CHILD_ADDED, window);
    DispatchEvent(&event);
}

void CWindow::RemoveChild( CWindow *window )
{
    BEATS_ASSERT(window);
    RemoveChild(window->Name());
}

void CWindow::RemoveChild( const TString &name )
{
    auto itr = std::find_if(m_children.begin(), m_children.end(), [&name](CWindow *wnd){
        return wnd->Name() == name;
    });
    if(itr != m_children.end())
    {
        WindowEvent event(EVENT_CHILD_REMOVE, *itr);
        DispatchEvent(&event);

        if(*itr == CWindowManager::GetInstance()->FocusedWindow())
            CWindowManager::GetInstance()->SetFocusedWindow(nullptr);

        m_children.erase(itr);
    }
}

void CWindow::DestroyChild(CWindow *window, bool delay)
{
    if(window)
    {
        CWindowManager::GetInstance()->DestroyWindow(window, delay);
    }
}

void CWindow::DestroyChild(const TString &name, bool delay)
{
    BEATS_ASSERT(GetChild(name));
    CWindowManager::GetInstance()->DestroyWindow(name, delay);
}

CWindow *CWindow::GetChild( const TString &name, bool recursively ) const
{
    CWindow* pRet = nullptr;
    auto itr = std::find_if(m_children.begin(), m_children.end(), [&name](CWindow *wnd){
        return wnd->Name() == name;
    });    
    if(itr != m_children.end())
    {
        pRet = *itr;
    }
    else if(recursively)
    {
        for(auto child : m_children)
        {
            CWindow *window = child->GetChild(name, recursively);
            if(window)
            {
                pRet = window;
                break;
            }
        }
    }
    return pRet;
}

CWindow *CWindow::GetChild(kmScalar x, kmScalar y, bool recursively) const
{
    CWindow *result = nullptr;
    if(!m_bClipping || HitTest(x, y, true))
    {
        for(auto child : m_children)
        {
            kmScalar xc = x;
            kmScalar yc = y;
            child->ParentToLocal(xc, yc);
            bool hitted = child->HitTest(xc, yc, true) > eHTR_HIT_NONE;
            if(hitted)
            {
                result = child;
            }
            if(recursively && (hitted || !child->m_bClipping))
            {
                CWindow *newresult = child->GetChild(xc, yc, recursively);
                if(newresult)
                {
                    result = newresult;
                }
            }
            if(result)
                break;
        }
    }
    return result;
}

const CWindow::WindowVisitor &CWindow::Traverse( const WindowVisitor &visitor )
{
    for(auto child : m_children)
    {
        if(visitor(child))
            break;
    }
    return visitor;
}
