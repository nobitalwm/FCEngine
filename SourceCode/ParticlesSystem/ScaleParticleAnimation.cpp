#include "stdafx.h"
#include "ScaleParticleAnimation.h"

namespace FCEngine
{
    CScaleParticleAnimation::CScaleParticleAnimation()
    {
        m_fBeginScale = 1.0;
        m_fEndScale = 1.0;
        m_fCurrentScale = 1.0f;
        m_animationType = eAT_SCALE;
    }

    CScaleParticleAnimation::~CScaleParticleAnimation()
    {

    }

    void CScaleParticleAnimation::ReflectData(CSerializer& serializer)
    {
        CParticleAnimationBase::ReflectData(serializer);
        DECLARE_PROPERTY( serializer, m_fBeginScale, true, 0xFFFFFFFF, _T("开始放缩倍数"), NULL, NULL, NULL );
        DECLARE_PROPERTY( serializer, m_fEndScale, true, 0xFFFFFFFF, _T("结束放缩倍数"), NULL, NULL, NULL );
    }

    const void* CScaleParticleAnimation::GetCurrentAnimationValue( float currentTime, float totalTime )
    {
        if ( totalTime > 0 )
        {
            float cursor = currentTime / totalTime;
            m_fCurrentScale = ( m_fEndScale - m_fBeginScale ) * cursor + m_fBeginScale;
        }
        return &m_fCurrentScale;
    }

    void CScaleParticleAnimation::SetInitAnimationValue( const void* pValue, unsigned int count )
    {
        BEATS_ASSERT( count == 2, _T( "pValue must not be two float type" ) );
        float* pRelValue = (float*)pValue;
        m_fBeginScale = pRelValue[ 0 ];
        m_fEndScale = pRelValue[ 1 ];
    }
};