#ifndef EVENT_EVENT_DISPATCHER_H__INCLUDE
#define EVENT_EVENT_DISPATCHER_H__INCLUDE

//forward declaration
class CBaseEvent;

class CEventDispatcher
{
    //type definition
public:
    typedef std::function<void(CBaseEvent *)> EventHandler;
    typedef std::vector<EventHandler> EventHandlerList;
    typedef std::map<int, EventHandlerList> EventHandlerMap;

public:
    CEventDispatcher();
    virtual ~CEventDispatcher();

    virtual void SubscribeEvent(int type, const EventHandler &handler);
    template <typename MemberFunc, typename ObjectType>
    void SubscribeEvent(int type, const MemberFunc &func, ObjectType *obj);
    virtual void DispatchEvent(CBaseEvent *event);

protected:
    EventHandlerMap m_handlers;
};

template <typename MemberFunc, typename ObjectType>
void CEventDispatcher::SubscribeEvent( int type, const MemberFunc &func, ObjectType *obj )
{
    SubscribeEvent(type, std::bind(func, obj, std::placeholders::_1));
}

#endif