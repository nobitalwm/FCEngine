#ifndef PARTICLE_BCONEPARTICLEEMITTERENTITY_H_INCLUDE
#define PARTICLE_BCONEPARTICLEEMITTERENTITY_H_INCLUDE

#include "ParticleEmitterEntity.h"

namespace FCEngine
{
    class CConeParticleEmitterEntity : public CParticleEmitterEntityBase
    {
        friend class CParticleEmitter;
        DECLARE_REFLECT_GUID(CConeParticleEmitterEntity, 0x1544C158, CParticleEmitterEntityBase)
    public:
        CConeParticleEmitterEntity();
        virtual ~CConeParticleEmitterEntity();

        virtual void ReflectData(CSerializer& serializer) override;
    private:

        virtual const kmVec3& GetBirthPositon( );

        virtual void SetEmitterPropty( const void* pValue );

        virtual const EEmitterType& GetEmitterType( ) const;
    private:
        float m_fRadius;// the radius of the cone's bottom face
    };
};

#endif // PARTICLE_BCONEPARTICLEEMITTERENTITY_H_INCLUDE
