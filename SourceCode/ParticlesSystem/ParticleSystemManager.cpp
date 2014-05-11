#include "stdafx.h"
#include "ParticleSystemManager.h"

namespace FCEngine
{
    CParticleSystemManager* CParticleSystemManager::m_pInstance = NULL;

    CParticleSystemManager::CParticleSystemManager()
    {
        
    }

    CParticleSystemManager::~CParticleSystemManager()
    {
        m_ParticleSystemVector.clear();
        m_ParticleSystemComponetVector.clear();
    }

    void CParticleSystemManager::Update( float dtt )
    {
        CheckParticleSystemInComponent( );
        Update( m_ParticleSystemVector, dtt );
        Update( m_ParticleSystemComponetVector, dtt );
    }

    void CParticleSystemManager::Update( const TParticleSystemVector& vector, float dtt )
    {
        for ( auto iter : vector )
        {
            iter->Update( dtt );
        }
    }

    void CParticleSystemManager::Render()
    {
        Render( m_ParticleSystemVector );
        Render( m_ParticleSystemComponetVector );
    }

    void CParticleSystemManager::Render( const TParticleSystemVector& vector )
    {
        for ( auto iter : vector )
        {
            iter->Render( );
        }
    }

    void CParticleSystemManager::CheckParticleSystemInComponent()
    {
        m_ParticleSystemComponetVector.clear();
        const std::map<size_t, std::map<size_t, CComponentBase*>*>* pInstanceMap = CComponentManager::GetInstance()->GetComponentInstanceMap();

        for ( auto iter : *pInstanceMap )
        {
            if ( iter.first == CParticleSystem::REFLECT_GUID )
            {
                std::map<size_t, CComponentBase*>* pParticleSystemMap =
                    iter.second;
                for ( auto iterator : *pParticleSystemMap )
                {
                    CParticleSystem* pParticle = (CParticleSystem*)iterator.second;
                    m_ParticleSystemComponetVector.push_back( pParticle );
                }
            }
        }
    }

    void CParticleSystemManager::AddParticleSystem( CParticleSystem* pParticleSystem )
    {
        m_ParticleSystemVector.push_back( pParticleSystem );
    }

};