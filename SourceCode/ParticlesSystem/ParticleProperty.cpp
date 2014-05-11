#include "stdafx.h"
#include "ParticleProperty.h"

namespace FCEngine
{
    CParticleProperty::CParticleProperty()
    {
        m_fWidth = 1;
        m_fHeight = 1;
        m_particalType = ePT_BILLBORD;
    }

    CParticleProperty::~CParticleProperty()
    {

    }

    void CParticleProperty::ReflectData(CSerializer& serializer)
    {
        CComponentBase::ReflectData(serializer);
        DECLARE_PROPERTY(serializer, m_particalType, true, 0xFFFFFFFF, _T("��������"), NULL, NULL, NULL);
        DECLARE_PROPERTY( serializer, m_fWidth, true, 0xFFFFFFFF, _T("���ӿ��"), NULL, NULL, NULL );
        DECLARE_PROPERTY( serializer, m_fHeight, true, 0xFFFFFFFF, _T("���Ӹ߶�"), NULL, NULL, NULL );
        DECLARE_DEPENDENCY_LIST( serializer, m_particleAnimations, _T("���Ӷ���"), eDT_Weak );
    }

    float CParticleProperty::GetWidth() const
    {
        return m_fWidth;
    }

    float CParticleProperty::GetHeight() const
    {
        return m_fHeight;
    }

    const EParticleType& CParticleProperty::GetParticleType() const
    {
        return m_particalType;
    }

    const TParticleAnimation& CParticleProperty::GetAnimations() const
    {
        return m_particleAnimations;
    }

};