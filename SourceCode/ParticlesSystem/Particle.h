#ifndef PARTICLE_PARTICLE_H_INCLUDE
#define PARTICLE_PARTICLE_H_INCLUDE

#include "ColorParticleAnimation.h"
#include "RotateParticleAnimation.h"
#include "ScaleParticleAnimation.h"
#include "ParticleProperty.h"

namespace FCEngine
{
    struct SParticle
    {
        float m_fSpeed;
        float m_fDecay;
        float m_fForcePower;
        float m_fLiveTime;
        kmVec3 m_vec3ForceDirection;
        kmVec3 m_vec3OriginPosition;
        kmVec3 m_vec3OriginDirection;

        SParticle( )
        {
            kmVec3Fill( &m_vec3OriginPosition, 0.0, 0.0, 0.0 );
            kmVec3Fill( &m_vec3OriginDirection, 0.0, 1.0, 0.0 );
            m_fSpeed = 1;
            m_fDecay = 0;
            m_fForcePower = 0;
            kmVec3Fill( &m_vec3ForceDirection, 0.0, -1.0, 0.0 );
            m_fLiveTime = 1000;
        }
        ~SParticle( )
        {

        }
    };
    class CParticleEmitter;
    class CParticleEntity
    {
        friend class CParticleEmitter;
    private:
        CParticleEntity( );
        void Update( float dtt );
        void ComputeVertices(CVertexPTC** vertices, unsigned short** indexBuffer, OUT unsigned int& idx );
        void SetOriginPosition( const kmVec3& birthPosition );
        void SetOriginDirection( const kmVec3& direction );
        void SetSpeed( float velocity );
        void SetDecay( float velocityDecay );
        void SetForcePower( float power );
        void SetForceDirection( const kmVec3& direction );
        void SetLiveTime( float time );

        bool IsAlive( );
        void Activate( );
        void SetEmitter( CParticleEmitter* pEmitter );

    public:
        ~CParticleEntity( );

    private:
        bool m_bIsAlive;
        kmVec3 m_vec3CurrentPosition;
        float m_fCurrentLiveTime;
        SParticle* m_pParticle;
        float m_fCurrentSpeed;
        kmMat4 m_rotateMat;
        CParticleEmitter* m_pParticleEmitter;
    };
};

#endif // Particle_H_INCLUDE
