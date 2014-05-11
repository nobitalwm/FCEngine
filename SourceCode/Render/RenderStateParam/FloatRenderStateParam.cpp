#include "stdafx.h"
#include "FloatRenderStateParam.h"
#include "Render/Renderer.h"
#include "Render/RenderState.h"

CFloatRenderStateParam::CFloatRenderStateParam()
{

}

CFloatRenderStateParam::~CFloatRenderStateParam()
{

}

void CFloatRenderStateParam::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_type, true, 0xFFFFFFFF, _T("����"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_fValue, true, 0xFFFFFFFF, _T("������ֵ"), NULL, NULL, NULL);
}

void CFloatRenderStateParam::Apply()
{
    CRenderer* pRender = CRenderer::GetInstance();
    switch (m_type)
    {
    case eFSP_ClearDepth:
        pRender->ClearDepth(m_fValue);
        break;
    case eFSP_DepthFar:
        pRender->DepthRange(pRender->GetCurrentState()->GetDepthNear(), m_fValue);
        break;
    case eFSP_DepthNear:
        pRender->DepthRange(m_fValue, pRender->GetCurrentState()->GetDepthFar());
        break;
    case eFSP_LineWidh:
        pRender->LineWidth(m_fValue);
        break;
    default:
        BEATS_ASSERT(false, _T("Unkonwn type of CFloatRenderStateParam"));
        break;
    }
}

ERenderState CFloatRenderStateParam::GetRenderStateType() const
{
    return eRS_FloatMode;
}

CFloatRenderStateParam::EFloatStateParam CFloatRenderStateParam::GetFloatParamType() const
{
    return m_type;
}

void CFloatRenderStateParam::SetFloatParamType( EFloatStateParam type )
{
    m_type = type;
}

void CFloatRenderStateParam::SetValue( float value )
{
    m_fValue = value;
}

float CFloatRenderStateParam::GetValue() const
{
    return m_fValue;
}

bool CFloatRenderStateParam::operator==( const CRenderStateParamBase& other ) const
{
    BEATS_ASSERT( other.GetRenderStateType() == eRS_FloatMode );
    CFloatRenderStateParam* pOther = ( CFloatRenderStateParam* )&other;
    return ( m_fValue == pOther->m_fValue && m_type == pOther->m_type );
}

bool CFloatRenderStateParam::operator!=( const CRenderStateParamBase& other ) const
{
    BEATS_ASSERT( other.GetRenderStateType() == eRS_FloatMode );
    CFloatRenderStateParam* pOther = ( CFloatRenderStateParam* )&other;
    return ( m_fValue != pOther->m_fValue || m_type != pOther->m_type );
}
