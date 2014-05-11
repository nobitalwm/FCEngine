#include "stdafx.h"
#include "ConeParticleEmitterEntity.h"

namespace FCEngine
{
    CConeParticleEmitterEntity::CConeParticleEmitterEntity()
    {
        m_fRadius = 1;
        m_emitterType = eET_CONE;
    }

    CConeParticleEmitterEntity::~CConeParticleEmitterEntity()
    {

    }

    void CConeParticleEmitterEntity::ReflectData(CSerializer& serializer)
    {
        CParticleEmitterEntityBase::ReflectData(serializer);
        DECLARE_PROPERTY( serializer, m_fRadius, true, 0xFFFFFFFF, _T("×¶·¢ÉäÆ÷µ×Ãæ°ë¾¶"), NULL, NULL, NULL );
    }

    const kmVec3& CConeParticleEmitterEntity::GetBirthPositon()
    {
        //calc cone bottom face position ==> y = 0
        float x = RANGR_RANDOM_FLOAT( -1.0f, 1.0f );
        float y = 0.0f;
        float z = RANGR_RANDOM_FLOAT( -1.0f, 1.0f );

        kmVec3 pPosition;
        kmVec3Fill( &pPosition, x, y, z );
        kmVec3Normalize( &pPosition, &pPosition );
        kmVec3Scale( &m_vec3OriginPos, &pPosition, m_fRadius );

        return m_vec3OriginPos;
    }

    void CConeParticleEmitterEntity::SetEmitterPropty( const void* pValue )
    {
        float* value = (float*)pValue;
        m_fRadius = *value;
        m_fRadius *= 0.5f;
    }

    const EEmitterType& CConeParticleEmitterEntity::GetEmitterType() const
    {
        return m_emitterType;
    }

};
