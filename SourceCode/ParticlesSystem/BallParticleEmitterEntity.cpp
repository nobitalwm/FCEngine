#include "stdafx.h"
#include "BallParticleEmitterEntity.h"

namespace FCEngine
{
    CBallParticleEmitterEntity::CBallParticleEmitterEntity()
    {
        m_fRadius = 1;
        m_emitterType = eET_BALL;
    }


    CBallParticleEmitterEntity::~CBallParticleEmitterEntity()
    {

    }

    void CBallParticleEmitterEntity::ReflectData(CSerializer& serializer)
    {
        CParticleEmitterEntityBase::ReflectData(serializer);
        DECLARE_PROPERTY( serializer, m_fRadius, true, 0xFFFFFFFF, _T("Çò·¢ÉäÆ÷°ë¾¶"), NULL, NULL, NULL );
    }

    const kmVec3& CBallParticleEmitterEntity::GetBirthPositon()
    {
        //Calculation position in the ball
        float x = RANGR_RANDOM_FLOAT( -1.0f, 1.0f );
        float y = RANGR_RANDOM_FLOAT( -1.0f, 1.0f );
        float z = RANGR_RANDOM_FLOAT( -1.0f, 1.0f );
        kmVec3 pPosition;
        kmVec3Fill( &pPosition, x, y, z );
        kmVec3Normalize( &pPosition, &pPosition );
        kmVec3Scale( &m_vec3OriginPos, &pPosition, m_fRadius );

        return m_vec3OriginPos;
    }

    void CBallParticleEmitterEntity::SetEmitterPropty( const void* pValue )
    {
        float* value = (float*)pValue;
        m_fRadius = *value;
    }

    const EEmitterType& CBallParticleEmitterEntity::GetEmitterType() const
    {
        return m_emitterType;
    }

};