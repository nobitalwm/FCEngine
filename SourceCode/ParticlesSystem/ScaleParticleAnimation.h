#ifndef PARTICLE_SCALEPARTICLEANIMATION_H_INCLUDE
#define PARTICLE_SCALEPARTICLEANIMATION_H_INCLUDE

#include "ParticleAnimationBase.h"

namespace FCEngine
{
    class CScaleParticleAnimation : public CParticleAnimationBase
    {
        friend class CParticleEntity;
        DECLARE_REFLECT_GUID( CScaleParticleAnimation, 0x1544B156, CParticleAnimationBase )
    public:
        CScaleParticleAnimation();
        virtual ~CScaleParticleAnimation();
        virtual void ReflectData(CSerializer& serializer) override;

    private:
        virtual const void* GetCurrentAnimationValue( float currentTime, float totalTime );
        virtual void SetInitAnimationValue( const void* pValue, unsigned int count = 1 );

    private:
        float m_fCurrentScale;
        float m_fBeginScale;
        float m_fEndScale;
    };
};

#endif // PARTICLE_SCALEPARTICLEANIMATION_H_INCLUDE