#ifndef PARTICLE_PARTICLEPROPERTY_H_INCLUDE
#define PARTICLE_PARTICLEPROPERTY_H_INCLUDE

#include "ParticleAnimationBase.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"

namespace FCEngine
{
    enum EParticleType
    {
        ePT_BILLBORD = 0,//aways face to camera
        ePT_AXISALIGNED//away face to one axis of the coordinate
    };

    typedef std::vector< CParticleAnimationBase* > TParticleAnimation;

    class CParticleProperty : public CComponentBase
    {
        friend class CParticleEmitter;
        friend class CParticleEntity;
        typedef std::vector< CParticleAnimationBase* > TParticleAnimation;
        DECLARE_REFLECT_GUID( CParticleProperty, 0x1544A158, CComponentBase )
    public:
        CParticleProperty( );
        virtual ~CParticleProperty( );

        virtual void ReflectData(CSerializer& serializer) override;
    private:

        float GetWidth( ) const;
        float GetHeight( ) const;
        const EParticleType& GetParticleType( ) const;
        const TParticleAnimation& GetAnimations( ) const;

    private:

        float m_fWidth;
        float m_fHeight;
        EParticleType m_particalType;
        TParticleAnimation m_particleAnimations;
    };
};

#endif // PARTICLE_PARTICLEPROPERTY_H_INCLUDE
