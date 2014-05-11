#include "stdafx.h"
#include "ParticleEmitterEntity.h"

namespace FCEngine
{
    CParticleEmitterEntityBase::CParticleEmitterEntityBase()
    {
        kmVec3Fill( &m_vec3OriginPos, 0, 0, 0 );
    }


    CParticleEmitterEntityBase::~CParticleEmitterEntityBase()
    {

    }

};
