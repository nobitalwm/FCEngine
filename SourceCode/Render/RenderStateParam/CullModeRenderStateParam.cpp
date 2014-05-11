#include "stdafx.h"
#include "CullModeRenderStateParam.h"
#include "Render/Renderer.h"

CCullModeRenderStateParam::CCullModeRenderStateParam()
{

}

CCullModeRenderStateParam::~CCullModeRenderStateParam()
{

}

void CCullModeRenderStateParam::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_nValue, true, 0xFFFFFFFF, _T("±³ÃæÌÞ³ýÄ£Ê½"), NULL, NULL, NULL);
}

void CCullModeRenderStateParam::Apply()
{
    CRenderer::GetInstance()->CullFace(m_nValue);
}

ERenderState CCullModeRenderStateParam::GetRenderStateType() const
{
    return eRS_CullMode;
}

void CCullModeRenderStateParam::SetCullModeType( ECullModeType type )
{
    m_nValue = type;
}

CCullModeRenderStateParam::ECullModeType CCullModeRenderStateParam::GetCullModeType() const
{
    return m_nValue;
}

bool CCullModeRenderStateParam::operator==( const CRenderStateParamBase& other ) const
{
    BEATS_ASSERT( other.GetRenderStateType() == eRS_CullMode );
    CCullModeRenderStateParam* pOther = (CCullModeRenderStateParam*)&other;
    return m_nValue == pOther->m_nValue;
}

bool CCullModeRenderStateParam::operator!=( const CRenderStateParamBase& other ) const
{
    BEATS_ASSERT( other.GetRenderStateType() == eRS_CullMode );
    CCullModeRenderStateParam* pOther = (CCullModeRenderStateParam*)&other;
    return m_nValue != pOther->m_nValue;
}
