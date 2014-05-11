#include "stdafx.h"
#include "ParticleAnimationBase.h"

namespace FCEngine
{

    CParticleAnimationBase::CParticleAnimationBase()
    {

    }

    CParticleAnimationBase::~CParticleAnimationBase()
    {

    }

    const EAnimationType& CParticleAnimationBase::GetAnimationType()
    {
        return m_animationType;
    }

}


