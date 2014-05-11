#ifndef PARTICLE_BALLPARTICLEEMITTERENTITY_H_INCLUDE
#define PARTICLE_BALLPARTICLEEMITTERENTITY_H_INCLUDE

#include "ParticleEmitterEntity.h"

namespace FCEngine
{
    class CBallParticleEmitterEntity : public CParticleEmitterEntityBase
    {
        friend class CParticleEmitter;
        DECLARE_REFLECT_GUID( CBallParticleEmitterEntity, 0x1544C156, CParticleEmitterEntityBase )
    public:
        CBallParticleEmitterEntity( );
        virtual ~CBallParticleEmitterEntity( );

        virtual void ReflectData(CSerializer& serializer) override;
    private:
        virtual const kmVec3& GetBirthPositon( );

        virtual void SetEmitterPropty( const void* pValue );

        virtual const EEmitterType& GetEmitterType( ) const;
    private:
        float m_fRadius;//the radius of the ball
    };
};

#endif // PARTICLE_BALLPARTICLEEMITTERENTITY_H_INCLUDE
