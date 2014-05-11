
#include "stdafx.h"
#include "ShaderUniformData.h"

CShaderUniformData::CShaderUniformData()
{
    m_fValue = 0.0f;
}

CShaderUniformData::~CShaderUniformData()
{

}

void CShaderUniformData::ReflectData(CSerializer& serializer)
{
    CComponentBase::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_fValue, true, 0xFFFFFFFF, _T("Êý¾Ý"), NULL, NULL, NULL);
}

void CShaderUniformData::SetValue( float data )
{
    m_fValue = data;
}

bool CShaderUniformData::operator==( const CShaderUniformData& other ) const
{
    return m_fValue == other.m_fValue;
}

bool CShaderUniformData::operator!=( const CShaderUniformData& other ) const
{
     return m_fValue != other.m_fValue;
}
