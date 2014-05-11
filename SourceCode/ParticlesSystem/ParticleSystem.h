#ifndef PARTICLE_PARTICLESYSYTEM_H_INCLUDE
#define PARTICLE_PARTICLESYSYTEM_H_INCLUDE

#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"
#include "Emitter.h"

namespace FCEngine
{
    class CParticleSystem : public CComponentBase
    {
        friend class ParticleSystemManager;
        friend class ParticleSystemScript;
        
        typedef std::vector< CParticleEmitter* > TEmitterVector;
        DECLARE_REFLECT_GUID( CParticleSystem, 0x154AC159, CComponentBase )
    public:
        CParticleSystem();

        virtual ~CParticleSystem();

        virtual void ReflectData(CSerializer& serializer) override;

        void Update( float dtt );

        void Render( );

        void SetMvp( const kmMat4& mvp );

    private:
        TEmitterVector m_emitterVector;
    };

};

#endif // ParticleSystem_H_INCLUDE
