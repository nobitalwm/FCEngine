#include "stdafx.h"
#include "EventDispatcher.h"
#include "BaseEvent.h"

CEventDispatcher::CEventDispatcher()
{

}

CEventDispatcher::~CEventDispatcher()
{

}

void CEventDispatcher::SubscribeEvent( int type, const EventHandler &handler )
{
    m_handlers[type].push_back(handler);
}

void CEventDispatcher::DispatchEvent( CBaseEvent *event )
{
    auto itr = m_handlers.find(event->Type());
    if(itr != m_handlers.end())
    {
        event->SetSource(this);
        for(auto handler : itr->second)
        {
            handler(event);
            if(event->Stopped())
                break;
        }
    }
}
