#include "stdafx.h"
#include "PointParticleEmitterEntity.h"

namespace FCEngine
{
    CPointParticleEmitterEntity::CPointParticleEmitterEntity()
    {
        kmVec3Fill( &m_vec3Pos, 0, 0, 0 );
        m_emitterType = eET_POINT;
    }

    CPointParticleEmitterEntity::~CPointParticleEmitterEntity()
    {

    }

    const kmVec3& CPointParticleEmitterEntity::GetBirthPositon()
    {
        kmVec3Fill( &m_vec3OriginPos, m_vec3Pos.x, m_vec3Pos.y, m_vec3Pos.z );

        return m_vec3OriginPos;
    }


    void CPointParticleEmitterEntity::SetEmitterPropty( const void* pValue )
    {
        kmVec3* value = (kmVec3*)pValue;
        kmVec3Fill( &m_vec3Pos, value->x, value->y, value->z );
    }

    const EEmitterType& CPointParticleEmitterEntity::GetEmitterType() const
    {
        return m_emitterType;
    }

};
