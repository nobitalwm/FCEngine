#include "stdafx.h"
#include "ColorParticleAnimation.h"

namespace FCEngine
{
    CColorParticleAnimation::CColorParticleAnimation()
    {
        m_currentColor = CColor( 0.0f, 0.0f, 0.0f, 0.0f );
        m_animationType = eAT_COLOR;
    }

    CColorParticleAnimation::~CColorParticleAnimation()
    {

    }

    void CColorParticleAnimation::ReflectData(CSerializer& serializer)
    {
        CParticleAnimationBase::ReflectData(serializer);
        DECLARE_PROPERTY( serializer, m_beginColor, true, 0xFFFFFFFF, _T("开始颜色"), NULL, NULL, NULL );
        DECLARE_PROPERTY( serializer, m_endColor, true, 0xFFFFFFFF, _T("结束颜色"), NULL, NULL, NULL );
    }

    const void* CColorParticleAnimation::GetCurrentAnimationValue( float currentTime, float totalTime )
    {
        if ( totalTime > 0 )
        {
            float delta = currentTime / totalTime;
            float r = CalcCurrentValue( m_beginColor.r, m_endColor.r, delta );
            float g = CalcCurrentValue( m_beginColor.g, m_endColor.g, delta );
            float b = CalcCurrentValue( m_beginColor.b, m_endColor.b, delta );
            float a = CalcCurrentValue( m_beginColor.a, m_endColor.a, delta );
            //Temporarily divided by 255， change it after editor complate
            m_currentColor = CColor( r / 255.0f , g / 255.0f , b / 255.0f , a / 255.0f );
        }
        return &m_currentColor;
    }

    void CColorParticleAnimation::SetInitAnimationValue( const void* pValue, unsigned int count )
    {
        BEATS_ASSERT( count == 2, _T( "pValue must not be two CColor type" ) );
        CColor* pColor = (CColor*)pValue;
        m_beginColor = pColor[ 0 ];
        m_endColor = pColor[ 1 ];
    }

    float CColorParticleAnimation::CalcCurrentValue( float begin, float end, float delta )
    {
        if ( delta < 0.0 )
        {
            delta = 0.0;
        }
        else if( delta > 1.0 )
        {
            delta = 1.0;
        }
        return ( end - begin ) * delta + begin;
    }
};
