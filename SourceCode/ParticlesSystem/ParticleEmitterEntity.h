#ifndef PARTICLE_PARTICLEEMITTERENTITY_H_INCLUDE
#define PARTICLE_PARTICLEEMITTERENTITY_H_INCLUDE

#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"

namespace FCEngine
{
    enum EEmitterType
    {
        eET_POINT = 0,
        eET_BOX,
        eET_BALL,
        eET_CONE
    };

    class CParticleEmitterEntityBase : public CComponentBase
    {
        friend class CParticleEmitter;
        DECLARE_REFLECT_GUID_ABSTRACT( CParticleEmitterEntityBase, 0x1544C155, CComponentBase )
    protected:
        CParticleEmitterEntityBase( );
        virtual ~CParticleEmitterEntityBase();

        //Get the particle birth position from any type 
        virtual const kmVec3& GetBirthPositon( ) = 0;
        virtual const EEmitterType& GetEmitterType( ) const = 0;
        virtual void SetEmitterPropty( const void* pValue ) = 0;

     protected:
        kmVec3 m_vec3OriginPos;
        EEmitterType m_emitterType;
    };
};

#endif // PARTICLE_PARTICLEEMITTERENTITY_H_INCLUDE
