#include "stdafx.h"
#include "Animatable.h"

using namespace FCGUI;

CAnimatable::CAnimatable()
{

}

CAnimatable::~CAnimatable()
{
    for(auto prop : m_props)
    {
        BEATS_SAFE_DELETE(prop.second);
    }
}

void CAnimatable::AddProp( int ID, const TString &name, 
                         const CAnimProp::Setter &setter, const CAnimProp::Getter &getter )
{
    BEATS_ASSERT(m_props.find(ID) == m_props.end());
    CAnimProp *prop = new CAnimProp(ID, name, setter, getter);
    m_props[ID] = prop;
}

const CAnimProp *CAnimatable::GetProp( int ID ) const
{
    auto itr = m_props.find(ID);
    return itr != m_props.end() ? itr->second : nullptr;
}
