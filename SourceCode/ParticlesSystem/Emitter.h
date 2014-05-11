#ifndef PARTICLE_EMITTER_H_INCLUDE
#define PARTICLE_EMITTER_H_INCLUDE

#include "Particle.h"
#include "Render/ShaderProgram.h"
#include "Render/Material.h"
#include "BallParticleEmitterEntity.h"
#include "PointParticleEmitterEntity.h"
#include "BoxParticleEmitterEntity.h"
#include "ConeParticleEmitterEntity.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"
#include "ParticleProperty.h"

namespace FCEngine
{
    class CParticleEmitter : public CComponentBase
    {
        friend class CParticleSystem;
        typedef std::list< CParticleEntity* > TParticleList;
        DECLARE_REFLECT_GUID( CParticleEmitter, 0x1544A159, CComponentBase )
    public:
        CParticleEmitter();
        ~CParticleEmitter( );

        virtual void ReflectData(CSerializer& serializer) override;
        const CParticleProperty* GetParticleProperty() const;
    private:

        void BeginShoot( );

        // Update the emitter before render
        void Update( float dtt );
        void Render( );
        void SetTotalCount( unsigned int count );
        unsigned int GetTotleCount( );
        void SetShootDeriction( const kmVec3& deriction );
        const kmVec3& GetShootDeriction( ) const;
        void SetShootOrigin( const kmVec3& origin );
        const kmVec3& GetShootOrigin( ) const;
        void SetShootDegree( const kmVec3& degree );
        const kmVec3& GetShootDegree( ) const;
        void SetShootVelocity( unsigned int velocity );
        unsigned int GetShootVelocity( );
        void SetMinParticleVelocity( float velocity );
        float GetMinParticleVelocity( );
        void SetMaxParticleVelocity( float velocity );
        float GetMaxParticleVelocity( );
        void SetMinParticleVelocityDecay( float decay );
        float GetMinParticleVelocityDecay( );
        void SetMaxParticleVelocityDecay( float decay );
        float GetMaxParticleVelocityDecay();
        void SetForcePower( float power );
        float GetForcePower( );
        void SetFroceDeriction( const kmVec3& deriction );
        const kmVec3& GetFroceDeriction( ) const;
        void SetMinLiveTime( float time );
        float GetMinLiveTime( );
        void SetMaxLiveTime( float time );
        float GetMaxLiveTime( );
        void SetDelayTime( float time );
        float GetDelayTime( );
        void SetParticleConstraint( bool bLocalCoordinate );
        bool GetParticleConstraint( );
        void SetPlayLoop( bool bLoop );
        bool GetPlayLoop( );

    private:

        void ShootParicle( unsigned int count );
        void SetParticleInitProperty( CParticleEntity** pParticleEntity );
        CParticleEntity* GetParticleFromRecycle( ); 
        void UpdateActiveParticles( float dtt );
        void InitVAOAndVBO( int count );

    private:

        bool m_bLocalCoord;
        bool m_bIsLoop;
        unsigned int m_uTotalCount;
        unsigned int m_uRatePerSecond;
        float m_fMinParticleSpeed;
        float m_fMaxParticleSpeed;
        float m_fMinSpeedDecay;
        float m_fMaxSpeedDecay;
        float m_fForcePower;
        float m_fMinLiveTime;
        float m_fMaxLiveTime;
        float m_fDelayTime;
        kmVec3 m_vec3Origin;
        kmVec3 m_vec3Direction;
        kmVec3 m_vec3DirectionOffset;
        kmVec3 m_vec3ForceDirection;

        CVertexPTC* m_pVertData;
        unsigned short* m_pIndexBuffer;
        TParticleList m_aliveParticleList;
        TParticleList m_particleRecycle;

        GLuint m_VAO;
        GLuint m_VBO[ 2 ];

        unsigned int m_uCurrentCount;
        float m_fBeginTime;
        float m_fMinPreShootTime;
        float m_fDeltaTime;
        bool m_bInit;

        CShaderProgram* m_pParticleProgram;
        //component use
        CMaterial* m_pMaterial;
        CParticleEmitterEntityBase* m_pEmitterBase;
        CParticleProperty* m_pParticleProperty;
    };
};

#endif // Emitter_H_INCLUDE
