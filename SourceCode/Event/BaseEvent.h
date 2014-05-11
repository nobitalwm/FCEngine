#ifndef EVENT_BASE_EVENT_H__INCLUDE
#define EVENT_BASE_EVENT_H__INCLUDE

class CEventDispatcher;

class CBaseEvent
{
public:
    CBaseEvent(int type)
        : m_nType(type)
        , m_bStopPropagation(false)
        , m_pSource(nullptr)
    {}

    virtual ~CBaseEvent(){}

    int Type() const
    {
        return m_nType;
    }

    void StopPropagation()
    {
        m_bStopPropagation = true;
    }

    bool Stopped() const
    {
        return m_bStopPropagation;
    }

    void SetSource(CEventDispatcher *source)
    {
        m_pSource = source;
    }

    CEventDispatcher *Source() const
    {
        return m_pSource;
    }

protected:
    int m_nType;
    bool m_bStopPropagation;
    CEventDispatcher *m_pSource;
};

#endif