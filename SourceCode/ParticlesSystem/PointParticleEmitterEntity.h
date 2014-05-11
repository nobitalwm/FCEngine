#ifndef PARTICLE_POINTPARTICLEEMITTERENTITY_H_INCLUDE
#define PARTICLE_POINTPARTICLEEMITTERENTITY_H_INCLUDE

#include "ParticleEmitterEntity.h"

namespace FCEngine
{
    class CPointParticleEmitterEntity : public CParticleEmitterEntityBase
    {
        friend class CParticleEmitter;
        DECLARE_REFLECT_GUID(CPointParticleEmitterEntity, 0x1544C159, CParticleEmitterEntityBase )
    public:
        CPointParticleEmitterEntity( );
        virtual ~CPointParticleEmitterEntity( );

    private:
        virtual const kmVec3& GetBirthPositon( );

        virtual void SetEmitterPropty( const void* pValue );

        virtual const EEmitterType& GetEmitterType( ) const;
    private:
        kmVec3 m_vec3Pos;
    };
};

#endif // PARTICLE_POINTPARTICLEEMITTERENTITY_H_INCLUDE
