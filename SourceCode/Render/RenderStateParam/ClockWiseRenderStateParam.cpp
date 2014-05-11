#include "stdafx.h"
#include "ClockWiseRenderStateParam.h"
#include "Render/Renderer.h"

CClockWiseRenderStateParam::CClockWiseRenderStateParam()
{

}

CClockWiseRenderStateParam::~CClockWiseRenderStateParam()
{

}

void CClockWiseRenderStateParam::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_nValue, true, 0xFFFFFFFF, _T("»æÖÆË³Ðò"), NULL, NULL, NULL);
}

void CClockWiseRenderStateParam::Apply()
{
    CRenderer::GetInstance()->FrontFace(m_nValue);
}

ERenderState CClockWiseRenderStateParam::GetRenderStateType() const
{
    return eRS_ClockWiseMode;
}

void CClockWiseRenderStateParam::SetClockWiseType( EClockWiseType type )
{
    m_nValue = type;
}

CClockWiseRenderStateParam::EClockWiseType CClockWiseRenderStateParam::GetClockWiseType() const
{
    return m_nValue;
}

bool CClockWiseRenderStateParam::operator==( const CRenderStateParamBase& other ) const
{
    BEATS_ASSERT( other.GetRenderStateType() == eRS_ClockWiseMode );
    CClockWiseRenderStateParam* pOther = (CClockWiseRenderStateParam*)&other;
    return m_nValue == pOther->m_nValue;
}

bool CClockWiseRenderStateParam::operator!=( const CRenderStateParamBase& other ) const
{
    BEATS_ASSERT( other.GetRenderStateType() == eRS_ClockWiseMode );
    CClockWiseRenderStateParam* pOther = (CClockWiseRenderStateParam*)&other;
    return m_nValue != pOther->m_nValue;
}
