#include "stdafx.h"
#include "FunctionRenderStateParam.h"
#include "Render/Renderer.h"

CFunctionRenderStateParam::CFunctionRenderStateParam()
{

}

CFunctionRenderStateParam::~CFunctionRenderStateParam()
{

}

void CFunctionRenderStateParam::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_type, true, 0xFFFFFFFF, _T("����"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_nValue, true, 0xFFFFFFFF, _T("�ȽϺ���"), NULL, NULL, NULL);
}

void CFunctionRenderStateParam::Apply()
{
    CRenderer* pRender = CRenderer::GetInstance();
    switch (m_type)
    {
    case eFSP_DepthFunction:
        pRender->DepthFunc(m_nValue);
        break;
    case eFSP_StencilFunction:
        pRender->StencilFunc(m_nValue);
        break;
    default:
        BEATS_ASSERT(false, _T("Unkonwn type of CFunctionRenderStateParam"));
        break;
    }
}

ERenderState CFunctionRenderStateParam::GetRenderStateType() const
{
    return eRS_FuncMode;
}

void CFunctionRenderStateParam::SetValue( EFunctionType value )
{
    m_nValue = value;
}

CFunctionRenderStateParam::EFunctionType CFunctionRenderStateParam::GetValue() const
{
    return m_nValue;
}

void CFunctionRenderStateParam::SetType( EFunctionStateParam type )
{
    m_type = type;
}

CFunctionRenderStateParam::EFunctionStateParam CFunctionRenderStateParam::GetType() const
{
    return m_type;
}

bool CFunctionRenderStateParam::operator==( const CRenderStateParamBase& other ) const
{
    BEATS_ASSERT( other.GetRenderStateType() == eRS_FuncMode );
    CFunctionRenderStateParam* pOther = ( CFunctionRenderStateParam* )&other;
    return ( m_nValue == pOther->m_nValue && m_type == pOther->m_type );
}

bool CFunctionRenderStateParam::operator!=( const CRenderStateParamBase& other ) const
{
    BEATS_ASSERT( other.GetRenderStateType() == eRS_FuncMode );
    CFunctionRenderStateParam* pOther = ( CFunctionRenderStateParam* )&other;
    return ( m_nValue != pOther->m_nValue || m_type != pOther->m_type );
}

