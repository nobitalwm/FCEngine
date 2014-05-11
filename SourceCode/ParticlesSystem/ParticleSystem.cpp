#include "stdafx.h"
#include "ParticleSystem.h"

namespace FCEngine
{
    CParticleSystem::CParticleSystem()
    {

    }

    CParticleSystem::~CParticleSystem()
    {
    }

    void CParticleSystem::ReflectData(CSerializer& serializer)
    {
        CComponentBase::ReflectData(serializer);
        DECLARE_DEPENDENCY_LIST( serializer, m_emitterVector, _T("Á£×Ó·¢ÉäÆ÷"), eDT_Strong );
    }

    void CParticleSystem::Update( float dtt )
    {
        for ( auto i : m_emitterVector )
        {
            i->Update( dtt );
        }
    }

    void CParticleSystem::Render()
    {
        for ( auto i : m_emitterVector )
        {
            i->Render( );
        }
    }

    void CParticleSystem::SetMvp( const kmMat4& mvp )
    {

    }

};
