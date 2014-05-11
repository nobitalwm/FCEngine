#include "stdafx.h"
#include "Particle.h"
#include "Emitter.h"

#define PARTICLE_MASS 10

namespace FCEngine
{
    CParticleEntity::CParticleEntity()
    {
        m_pParticle = new SParticle( );
        m_pParticleEmitter = NULL;
        Activate();
    }

    CParticleEntity::~CParticleEntity()
    {
        BEATS_SAFE_DELETE( m_pParticle );
    }

    void CParticleEntity::Update( float dtt )
    {
        m_fCurrentLiveTime += dtt;
        float updateTime = dtt;//calculation the update time
        if ( m_fCurrentLiveTime > m_pParticle->m_fLiveTime )
        {
            updateTime = dtt - ( m_fCurrentLiveTime - m_pParticle->m_fLiveTime );
            m_bIsAlive = false;//the particle is die
        }
        //calculation the run direction
        //calculation the run distance
        kmVec3 pMove;
        m_fCurrentSpeed -= m_pParticle->m_fDecay * dtt;
        if ( m_fCurrentSpeed <= 0 )
        {
            m_fCurrentSpeed = 0;
        }
        float distance = m_fCurrentSpeed * dtt;
        //calculation the position
        kmVec3Scale( &pMove, &m_pParticle->m_vec3OriginDirection, distance ); 
        
        // s = v0 * t + 0.5 * a * t * t, a = f / m(m is the mass)
        if ( m_pParticle->m_fForcePower != 0 )
        {
            float length = 0.5f * m_pParticle->m_fForcePower / PARTICLE_MASS * m_fCurrentLiveTime * m_fCurrentLiveTime;
            kmVec3 direction;
            kmVec3Scale( &direction, &m_pParticle->m_vec3ForceDirection, length );
            kmVec3Add( &pMove, &pMove, &direction );
        }
        kmVec3Add( &m_vec3CurrentPosition, &m_vec3CurrentPosition, &pMove );
    }

    void CParticleEntity::ComputeVertices( CVertexPTC** vertices, unsigned short** indexBuffer, OUT unsigned int& idx )
    {
        BEATS_ASSERT( NULL != m_pParticleEmitter, _T( "The particle property not be set" ));
        const CParticleProperty* pParticleProperty = m_pParticleEmitter->GetParticleProperty();
        if ( pParticleProperty )
        {
            //calculation the position in the local coordinate
            CVertexPTC* pVertices = *vertices;
            unsigned short* pIndexBuffer = *indexBuffer;
            const TParticleAnimation& aniamtions = pParticleProperty->GetAnimations( );
            kmVec3 leftTop, leftDown, rightTop, rightDown;
            kmVec3Fill( &leftTop, pParticleProperty->GetWidth() * -0.5f, pParticleProperty->GetHeight() * 0.5f, 0 );
            kmVec3Fill( &leftDown, pParticleProperty->GetWidth() * -0.5f, pParticleProperty->GetHeight() * -0.5f, 0 );
            kmVec3Fill( &rightTop, pParticleProperty->GetWidth() * 0.5f , pParticleProperty->GetHeight() * 0.5f, 0 );
            kmVec3Fill( &rightDown, pParticleProperty->GetWidth() * 0.5f, pParticleProperty->GetHeight() * -0.5f, 0 );
            for ( auto iter : aniamtions )
            {
                if ( eAT_SCALE == iter->GetAnimationType( ) )//scale
                {
                    const float* scale = (const float*)iter->GetCurrentAnimationValue( m_fCurrentLiveTime, m_pParticle->m_fLiveTime );
                    kmVec3Scale( &leftTop, &leftTop, *scale );
                    kmVec3Scale( &leftDown, &leftDown, *scale );
                    kmVec3Scale( &rightTop, &rightTop, *scale );
                    kmVec3Scale( &rightDown, &rightDown, *scale );
                }
                else if( eAT_ROTATE == iter->GetAnimationType( ) )//rotate
                {
                    const float* rotate = (const float*)iter->GetCurrentAnimationValue( m_fCurrentLiveTime, m_pParticle->m_fLiveTime );
                    kmMat4RotationPitchYawRoll( &m_rotateMat, 0, 0, *rotate );
                    kmVec3Transform( &leftTop, &leftTop, &m_rotateMat );
                    kmVec3Transform( &leftDown, &leftDown, &m_rotateMat );
                    kmVec3Transform( &rightTop, &rightTop, &m_rotateMat );
                    kmVec3Transform( &rightDown, &rightDown, &m_rotateMat );
                }
                else if( eAT_COLOR == iter->GetAnimationType( ) )
                {
                    //color
                    const CColor* color = (const CColor*)iter->GetCurrentAnimationValue( m_fCurrentLiveTime, m_pParticle->m_fLiveTime );
                    pVertices[ idx ].color = *color;
                    pVertices[ idx + 1 ].color = *color;
                    pVertices[ idx + 2 ].color = *color;
                    pVertices[ idx + 3 ].color = *color;
                }
            }

            //translate
            kmVec3Add( &leftTop, &leftTop, &m_vec3CurrentPosition );
            kmVec3Add( &leftDown, &leftDown, &m_vec3CurrentPosition );
            kmVec3Add( &rightTop, &rightTop, &m_vec3CurrentPosition );
            kmVec3Add( &rightDown, &rightDown, &m_vec3CurrentPosition );

            //position
            kmVec3Fill( &pVertices[ idx ].position, leftTop.x, leftTop.y, leftTop.z );
            kmVec3Fill( &pVertices[ idx + 1 ].position, leftDown.x, leftDown.y, leftDown.z );
            kmVec3Fill( &pVertices[ idx + 2 ].position, rightDown.x, rightDown.y, rightDown.z );
            kmVec3Fill( &pVertices[ idx + 3 ].position, rightTop.x, rightTop.y, rightTop.z );
            //texture
            pVertices[ idx ].tex.u = 0;
            pVertices[ idx ].tex.v = 0;
            pVertices[ idx + 1 ].tex.u = 0;
            pVertices[ idx + 1 ].tex.v = 1;
            pVertices[ idx + 2 ].tex.u = 1;
            pVertices[ idx + 2 ].tex.v = 1;
            pVertices[ idx + 3 ].tex.u = 1;
            pVertices[ idx + 3 ].tex.v = 0;

            //index
            unsigned short indexIdx = ( unsigned short )idx / 4 * 6;
            pIndexBuffer[ indexIdx ] = ( unsigned short )idx;
            pIndexBuffer[ indexIdx + 1 ] = ( unsigned short )idx + 1;
            pIndexBuffer[ indexIdx + 2 ] = ( unsigned short )idx + 2;
            pIndexBuffer[ indexIdx + 3 ] = ( unsigned short )idx;
            pIndexBuffer[ indexIdx + 4 ] = ( unsigned short )idx + 2;
            pIndexBuffer[ indexIdx + 5 ] = ( unsigned short )idx + 3;

            idx += 4;
        }
    }

    void CParticleEntity::SetOriginPosition( const kmVec3& birthPosition )
    {
        kmVec3Fill( &m_pParticle->m_vec3OriginPosition, birthPosition.x, birthPosition.y, birthPosition.z );
        kmVec3Fill( &m_vec3CurrentPosition, birthPosition.x, birthPosition.y, birthPosition.z );
    }

    void CParticleEntity::SetOriginDirection( const kmVec3& direction )
    {
        kmVec3Fill( &m_pParticle->m_vec3OriginDirection, direction.x, direction.y, direction.z );
        if ( kmVec3Length( &m_pParticle->m_vec3OriginDirection ) > 0 )
        {
            kmVec3Normalize( &m_pParticle->m_vec3OriginDirection , &m_pParticle->m_vec3OriginDirection );
        }
    }

    void CParticleEntity::SetSpeed( float velocity )
    {
        m_pParticle->m_fSpeed = velocity;
        m_fCurrentSpeed = velocity;
    }

    void CParticleEntity::SetDecay( float velocityDecay )
    {
        m_pParticle->m_fDecay = velocityDecay;
    }

    void CParticleEntity::SetForcePower( float power )
    {
        m_pParticle->m_fForcePower = power;
    }

    void CParticleEntity::SetForceDirection( const kmVec3& direction )
    {
        kmVec3Fill( &m_pParticle->m_vec3ForceDirection, direction.x, direction.y, direction.z );
        kmVec3Normalize( &m_pParticle->m_vec3ForceDirection , &m_pParticle->m_vec3ForceDirection );
    }

    void CParticleEntity::SetLiveTime( float time )
    {
        m_pParticle->m_fLiveTime = time;
    }

    bool CParticleEntity::IsAlive()
    {
        return m_bIsAlive;
    }

    void CParticleEntity::Activate()
    {
        m_bIsAlive = true;
        kmVec3Fill( &m_vec3CurrentPosition, 0.0, 0.0, 0.0 );
        m_fCurrentLiveTime = 0;
        kmMat4Identity( &m_rotateMat );
    }

    void CParticleEntity::SetEmitter( CParticleEmitter* pEmitter )
    {
        m_pParticleEmitter = pEmitter;
    }


};
