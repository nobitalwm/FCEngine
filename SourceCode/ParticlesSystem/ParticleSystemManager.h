#ifndef PARTICLE_PARTICLESYSYTEMMANAGER_H_INCLUDE
#define PARTICLE_PARTICLESYSYTEMMANAGER_H_INCLUDE

#include "ParticleSystem.h"

namespace FCEngine
{
    class CParticleSystemManager
    {
        BEATS_DECLARE_SINGLETON(CParticleSystemManager);
        typedef std::vector< CParticleSystem* > TParticleSystemVector;

    public:
        void Update( float dtt );
        void Render( );

        void AddParticleSystem( CParticleSystem* pParticleSystem );
    private:
        void CheckParticleSystemInComponent( );

        void Update( const TParticleSystemVector& vector, float dtt );

        void Render( const TParticleSystemVector& vector );

    private:
        TParticleSystemVector m_ParticleSystemVector;
        TParticleSystemVector m_ParticleSystemComponetVector;
    };
};

#endif // PARTICLE_PARTICLESYSYTEMMANAGER_H_INCLUDE
