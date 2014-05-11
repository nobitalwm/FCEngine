#ifndef GUI_WINDOW_WINDOW_H__INCLUDE
#define GUI_WINDOW_WINDOW_H__INCLUDE

#include "Event/EventDispatcher.h"
#include "GUI/GUIEventType.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"
#include "WindowDefs.h"
#include "GUI/Animation/Animatable.h"

//forward declaration
class CMouseEvent;
class CKeyboardEvent;

namespace FCGUI
{
    //forward declaration
    class CBaseRenderer;
    class CBaseLayout;

    class CWindow : public CEventDispatcher, public CComponentBase, public CAnimatable
    {
        typedef CComponentBase super;
        DECLARE_REFLECT_GUID(CWindow, 0xB4ADE61C, CComponentBase)
        DEFINE_WINDOW_TYPE(eWT_WINDOW_BASE);
        //type definition
    public:
        typedef std::vector<CWindow *> WindowListType;
        typedef std::function<bool(CWindow *)> WindowVisitor;

        //event definition
    public:
        enum EEventType
        {
            eET_EVENT_WINDOW = FCGUI::eGUIET_EVENT_WINDOW,
            eET_EVENT_INITIALIZED,
            eET_EVENT_DESTROY,
            eET_EVENT_REGISTERED,
            eET_EVENT_UNREGISTERED,
            eET_EVENT_MOVED,
            eET_EVENT_SIZED,
            eET_EVENT_ROTATED,
            eET_EVENT_SCALED,
            eET_EVENT_ANCHOR_CHANGED,
            eET_EVENT_VISIBLE_CHANGED,

            EVENT_CHILD_ADDED,
            EVENT_CHILD_REMOVE,
        };

        enum EHitTestResult
        {
            eHTR_HIT_NONE,
            eHTR_HIT_CONTENT,
            eHTR_HIT_TOP_LEFT,
            eHTR_HIT_TOP_RIGHT,
            eHTR_HIT_BOTTOM_LEFT,
            eHTR_HIT_BOTTOM_RIGHT,
            eHTR_HIT_ANCHOR,
            eHTR_HIT_ROTATION_HANDLE,
        };

        enum EStateType
        {
            eST_STATE_NORMAL,
            eST_STATE_DISABLED,
        };

        enum EPropertyType
        {
            ePT_PROP_X,
            ePT_PROP_Y,
            ePT_PROP_PERCENT_X,
            ePT_PROP_PERCENT_Y,
            ePT_PROP_WIDTH,
            ePT_PROP_HEIGHT,
            ePT_PROP_PERCENT_WIDTH,
            ePT_PROP_PERCENT_HEIGHT,
            ePT_PROP_ANCHOR_X,
            ePT_PROP_ANCHOR_Y,
            ePT_PROP_ROTATION,
            ePT_PROP_SCALE_X,
            ePT_PROP_SCALE_Y,

            ePT_PROP_COUNT,
        };

    public:
        CWindow();
        CWindow(const TString &name);
        virtual ~CWindow();

        virtual void ReflectData(CSerializer& serializer) override;
        virtual void OnPropertyChanged( const TString &variableName, void *value );
        void Init(const TString &name = _T(""));
        void SetPosSize(kmScalar x, kmScalar y, kmScalar xPercent, kmScalar yPercent,
            kmScalar width, kmScalar height, kmScalar widthPercent, kmScalar heightPercent);
        void SetArea(kmScalar left, kmScalar top, kmScalar leftPercent, kmScalar topPercent,
            kmScalar right, kmScalar bottom, kmScalar rightPercent, kmScalar bottomPercent);

        //component
        void SetRenderer(CBaseRenderer *renderer);
        CBaseRenderer *Renderer() const;
        void SetLayout(CBaseLayout *layout);
        CBaseLayout *Layout() const;

        //attribute
        void SetName(const TString &name);
        TString Name() const;
        void SetPos(kmScalar x, kmScalar y);
        void SetPos(kmVec2 pos);
        kmVec2 Pos() const;
        void SetPercentPos(kmScalar x, kmScalar y);
        void SetPercentPos(kmVec2 posPercent);
        kmVec2 PercentPos() const;
        kmVec2 RealPos() const;    //calculated from pos, pospercent, and parentsize
        void SetSize(kmScalar width, kmScalar height);
        void SetSize(kmVec2 size);
        kmVec2 Size() const;
        void SetPercentSize(kmScalar width, kmScalar height);
        void SetPercentSize(kmVec2 sizePercent);
        kmVec2 PercentSize() const;
        kmVec2 RealSize() const;    //calculated from size, sizepercent, and parentsize
        void SetAnchor(kmScalar x, kmScalar y);
        void SetAnchor(kmVec2 anchor);
        kmVec2 Anchor() const;
        kmVec2 RealAnchor() const;
        void SetRotation(kmScalar rotation);
        kmScalar Rotation() const;
        bool IsRotated() const;
        void SetScale(kmScalar scaleX, kmScalar scaleY);
        void SetScale(kmVec2 scale);
        kmVec2 Scale() const;
        void Scroll(kmVec2 offset);

        const kmMat4 &Transform() const;
        kmMat4 WorldTransform() const;
        void WorldToLocal(kmVec3 &pos) const;
        void WorldToLocal(kmScalar &x, kmScalar &y) const;
        void ParentToLocal(kmVec3 &pos) const;
        void ParentToLocal(kmScalar &x, kmScalar &y) const;
        void LocalToParent(kmVec3 &pos) const;
        void LocalToParent(kmScalar &x, kmScalar &y) const;
        void LocalToWorld(kmVec3 &pos) const;
        void LocalToWorld(kmScalar &x, kmScalar &y) const;

        EHitTestResult HitTest(kmScalar x, kmScalar y, bool localized = false) const;

        void Show();
        void Hide();
        void ToggleVisible();
        void SetVisible(bool visible);
        bool IsVisible() const;

        void Enable();
        void Disable();
        bool IsEnabled() const;

        void SetClipping(bool clipping);
        bool IsClipping() const;
        void GetScissorRect(kmVec2 &pos, kmVec2 &size);

        virtual int CurrState() const;

        void AddAutoAnimation(int eventType, const TString &animName);

        //event
        virtual bool OnMouseEvent(CMouseEvent *event);
        virtual bool OnKeyboardEvent(CKeyboardEvent *event);
        virtual void Update(float deltaTime);
        virtual void DispatchEvent(CBaseEvent *event) override;

        //hierachy
        CWindow *Parent() const;
        void AddChild(CWindow *window);
        void RemoveChild(CWindow *window);
        void RemoveChild(const TString &name);
        void DestroyChild(CWindow *window, bool delay = false);
        void DestroyChild(const TString &name, bool delay = false);
        CWindow *GetChild(const TString &name, bool recursively = true) const;
        CWindow *GetChild(kmScalar x, kmScalar y, bool recursively = true) const;
        const WindowVisitor &Traverse(const WindowVisitor &visitor);

    protected:
        void SetParent(CWindow *parent);
        void OnParentSized();
        void CalcRealPos();
        void CalcRealSize();
        void OnScrollOffset();
        void CalcChildRealPos();
        void CalcTransform();

    protected:
        //attribute
        TString m_strName;
        kmVec2 m_pos;
        kmVec2 m_percentPos;
        kmVec2 m_realPos;
        kmVec2 m_size;
        kmVec2 m_percentSize;
        kmVec2 m_realSize;
        kmVec2 m_anchor;
        kmScalar m_fRotation;
        kmVec2 m_scale;
        kmVec2 m_scrolloffset;
        bool m_bVisible;
        bool m_bEnabled;
        bool m_bClipping;
        kmMat4 m_transform;  //transform in parent space

        //component
        CBaseLayout *m_pLayout;
        CBaseRenderer *m_pRenderer;

        std::map<int, TString> m_autoAnimations;

        //window hierachy
        CWindow *m_pParent;
        WindowListType m_children;
    };
}

#endif