#ifndef PARTICLE_COLORPARTICLEANIMATION_H_INCLUDE
#define PARTICLE_COLORPARTICLEANIMATION_H_INCLUDE

#include "ParticleAnimationBase.h"

namespace FCEngine
{
    class CColorParticleAnimation : public CParticleAnimationBase
    {
        friend class CParticleEntity;
        DECLARE_REFLECT_GUID( CColorParticleAnimation, 0x1544B158, CParticleAnimationBase )
    public:
        CColorParticleAnimation();
        virtual ~CColorParticleAnimation();

        virtual void ReflectData(CSerializer& serializer) override;
    private:

        virtual const void* GetCurrentAnimationValue( float currentTime, float totalTime );

        virtual void SetInitAnimationValue( const void* pValue, unsigned int count = 1 );

        float CalcCurrentValue( float begin, float end, float delta );
    private:
        CColor m_currentColor;
        CColor m_beginColor;
        CColor m_endColor;
    };
};

#endif // PARTICLE_COLORPARTICLEANIMATION_H_INCLUDE
