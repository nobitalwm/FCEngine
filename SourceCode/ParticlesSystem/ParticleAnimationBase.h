#ifndef PARTICLE_PARTICLEANIMATIONBASE_H_INCLUDE
#define PARTICLE_PARTICLEANIMATIONBASE_H_INCLUDE

#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"

namespace FCEngine
{
    enum EAnimationType
    {
        eAT_COLOR = 0,
        eAT_SCALE,
        eAT_ROTATE
    };

    class CParticleAnimationBase : public CComponentBase
    {
        friend class CParticleEntity;
        DECLARE_REFLECT_GUID_ABSTRACT( CParticleAnimationBase, 0x1544B159, CComponentBase )
    public:
        virtual ~CParticleAnimationBase();

    protected:
        CParticleAnimationBase();

        // this method return void* ,the type decision by the ParticleAnimation,
        // when use this method must be care the type
        virtual const void* GetCurrentAnimationValue( float currentTime, float totalTime ) = 0;
        virtual void SetInitAnimationValue( const void* pValue, unsigned int count = 1 ) = 0;

        const EAnimationType& GetAnimationType( );

    protected:
        EAnimationType m_animationType;
    };
};

#endif // ParticleAnimationBase_H_INCLUDE