#include "stdafx.h"
#include "BoxParticleEmitterEntity.h"

namespace FCEngine
{
    CBoxParticleEmitterEntity::CBoxParticleEmitterEntity()
    {
        kmVec3Fill( &m_boxSize, 1, 1, 1 );
        m_emitterType = eET_BOX;
    }

    CBoxParticleEmitterEntity::~CBoxParticleEmitterEntity()
    {

    }

    void CBoxParticleEmitterEntity::ReflectData(CSerializer& serializer)
    {
        CParticleEmitterEntityBase::ReflectData(serializer);
        DECLARE_PROPERTY(serializer, m_boxSize, true, 0xFFFFFFFF, _T("盒子发射器大小"), NULL, NULL, NULL);
    }
    const kmVec3& CBoxParticleEmitterEntity::GetBirthPositon()
    {
        //Calculation position in the box
        float x = RANGR_RANDOM_FLOAT( -1.0f, 1.0f );
        x *= m_boxSize.x;
        float y = RANGR_RANDOM_FLOAT( -1.0f, 1.0f );
        y *= m_boxSize.y;
        float z = RANGR_RANDOM_FLOAT( -1.0f, 1.0f );
        z *= m_boxSize.z;
        kmVec3Fill( &m_vec3OriginPos, x, y, z );

        return m_vec3OriginPos;
    }


    void CBoxParticleEmitterEntity::SetEmitterPropty( const void* pValue )
    {
        kmVec3* value = (kmVec3*)pValue;
        kmVec3Fill( &m_boxSize, value->x, value->y, value->z );
    }

    const EEmitterType& CBoxParticleEmitterEntity::GetEmitterType() const
    {
        return m_emitterType;
    }

};