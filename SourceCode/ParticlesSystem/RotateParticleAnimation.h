#ifndef PARTICLE_ROTATEPARTICLEANIMATION_H_INCLUDE
#define PARTICLE_ROTATEPARTICLEANIMATION_H_INCLUDE

#include "ParticleAnimationBase.h"

namespace FCEngine
{
    class CRotateParticleAnimation : public CParticleAnimationBase
    {
        friend class CParticleEntity;
        DECLARE_REFLECT_GUID( CRotateParticleAnimation, 0x1544B157, CParticleAnimationBase )
    public:
        CRotateParticleAnimation();
        virtual ~CRotateParticleAnimation();

        virtual void ReflectData(CSerializer& serializer) override;
    private:
        virtual const void* GetCurrentAnimationValue( float currentTime, float totalTime );
        virtual void SetInitAnimationValue( const void* pValue, unsigned int count = 1 );
        CRotateParticleAnimation& operator=( const CRotateParticleAnimation& other );
    private:
        float m_fCurrentRotate;
        float m_fBaseRotate;
    };
};

#endif // PARTICLE_ROTATEPARTICLEANIMATION_H_INCLUDE
