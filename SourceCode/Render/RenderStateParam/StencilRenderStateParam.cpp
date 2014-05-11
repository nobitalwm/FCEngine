#include "stdafx.h"
#include "StencilRenderStateParam.h"
#include "Render/Renderer.h"

CStencilRenderStateParam::CStencilRenderStateParam()
{

}

CStencilRenderStateParam::~CStencilRenderStateParam()
{

}

void CStencilRenderStateParam::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_nFail, true, 0xFFFFFFFF, _T("失败模式"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_nZFail, true, 0xFFFFFFFF, _T("Z失败模式"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_nZPass, true, 0xFFFFFFFF, _T("Z成功模式"), NULL, NULL, NULL);
}

void CStencilRenderStateParam::Apply()
{
    CRenderer::GetInstance()->StencilOp(m_nFail, m_nZFail, m_nZPass);
}

ERenderState CStencilRenderStateParam::GetRenderStateType() const
{
    return eRS_StencilMode;
}

bool CStencilRenderStateParam::operator==( const CRenderStateParamBase& other ) const
{
    BEATS_ASSERT( other.GetRenderStateType() == eRS_StencilMode );
    CStencilRenderStateParam* pOther = ( CStencilRenderStateParam* )&other;
    return ( m_nZFail == pOther->m_nZFail && m_nFail == pOther->m_nFail && m_nZPass == pOther->m_nZPass );
}

void CStencilRenderStateParam::SetValue( EStencilType fail, EStencilType zFali, EStencilType zPass )
{
    m_nFail = fail;
    m_nZFail = zFali;
    m_nZPass = zPass;
}

void CStencilRenderStateParam::GetValue( EStencilType& fail, EStencilType& zFali, EStencilType& zPass ) const
{
    fail = m_nFail;
    zFali = m_nZFail;
    zPass = m_nZPass;
}

bool CStencilRenderStateParam::operator!=( const CRenderStateParamBase& other ) const
{
    BEATS_ASSERT( other.GetRenderStateType() == eRS_StencilMode );
    CStencilRenderStateParam* pOther = ( CStencilRenderStateParam* )&other;
    return ( m_nZFail != pOther->m_nZFail || m_nFail != pOther->m_nFail || m_nZPass != pOther->m_nZPass );
}
