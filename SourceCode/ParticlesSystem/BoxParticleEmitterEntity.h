#ifndef PARTICLE_BOXPARTICLEEMITTERENTITY_H_INCLUDE
#define PARTICLE_BOXPARTICLEEMITTERENTITY_H_INCLUDE

#include "ParticleEmitterEntity.h"

namespace FCEngine
{
    class CBoxParticleEmitterEntity : public CParticleEmitterEntityBase
    {
        DECLARE_REFLECT_GUID( CBoxParticleEmitterEntity, 0x1544C157, CParticleEmitterEntityBase )
    public:
        CBoxParticleEmitterEntity( );
        ~CBoxParticleEmitterEntity( );

        virtual void ReflectData(CSerializer& serializer) override;
    private:
        virtual const kmVec3& GetBirthPositon( );

        virtual void SetEmitterPropty( const void* pValue );

        virtual const EEmitterType& GetEmitterType( ) const;
    private:
        kmVec3 m_boxSize;//the size of the box
    };
};

#endif // PARTICLE_BOXPARTICLEEMITTERENTITY_H_INCLUDE
