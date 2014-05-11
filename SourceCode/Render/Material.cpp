#include "stdafx.h"
#include "Material.h"
#include "RenderManager.h"
#include "RenderState.h"
#include "Utility/BeatsUtility/Serializer.h"
#include "Renderer.h"
#include "Resource/ResourceManager.h"
#include "Shader.h"
#include "ShaderProgram.h"

CMaterial::CMaterial( bool bCreateRenderState )
    : m_pRenderState(NULL)
{
    m_ambientColor = CColor( 0.2f, 0.2f, 0.2f, 1.0f );
    m_diffuseColor = CColor( 0.8f, 0.8f, 0.8f, 1.0f );
    m_specularColor = CColor( 0.0f, 0.0f, 0.0f, 1.0f );
    m_fShininess = 0;
    if ( bCreateRenderState )
    {
        m_pRenderState = new CRenderState();
    }
}

CMaterial::~CMaterial()
{
    if ( m_pRenderState )
    {
        if ( -1 == m_pRenderState->GetId() )
        {
            BEATS_SAFE_DELETE( m_pRenderState );
        }
    }
}

void CMaterial::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_ambientColor, true, 0xFFFFFFFF, _T("环境反射"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_diffuseColor, true, 0xFFFFFFFF, _T("漫反射"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_specularColor, true, 0xFFFFFFFF, _T("镜面反射"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_fShininess, true, 0xFFFFFFFF, _T("耀光度"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_uniforms, true, 0xFFFFFFFF, _T("shader自定义参数"), NULL, NULL, NULL);
    DECLARE_DEPENDENCY(serializer, m_pRenderState, _T("渲染状态"), eDT_Weak);
}

bool CMaterial::Load()
{
    return true;
}

bool CMaterial::Unload()
{
    return false;
}

void CMaterial::Use()
{
    CRenderer* pRenderer = CRenderer::GetInstance();
    if ( m_pRenderState )
    {
        m_pRenderState->Restore();
        for ( size_t i = 0; i < m_textures.size(); ++i )
        {
            pRenderer->ActiveTexture( GL_TEXTURE0 + i );
            pRenderer->TexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            pRenderer->TexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            pRenderer->TexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            pRenderer->TexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
            pRenderer->BindTexture( GL_TEXTURE_2D, m_textures[ i ]->ID() );
            GLuint currProgram = pRenderer->GetCurrentState()->GetShaderProgram();
            if ( 0 != currProgram )
            {
                GLint textureLocation = pRenderer->GetUniformLocation(currProgram, COMMON_UNIFORM_NAMES[UNIFORM_TEX0 + i]);
                pRenderer->SetUniform1i( textureLocation, i);
            }
            FC_CHECK_GL_ERROR_DEBUG();
        }
        SendLightInfo( m_pRenderState );
        SendUniform();
    }
}

void CMaterial::SendUniform()
{
    for ( auto iter : m_uniforms )
    {
        iter->SendUniform( );
    }
}

void CMaterial::SendLightInfo(CRenderState* pState)
{
    float ambientColor[4] = {(float)m_ambientColor.r / 0xFF, (float)m_ambientColor.g / 0xFF, (float)m_ambientColor.b / 0xFF, (float)m_ambientColor.a / 0xFF};
    float diffuseColor[4] = {(float)m_diffuseColor.r / 0xFF, (float)m_diffuseColor.g / 0xFF, (float)m_diffuseColor.b / 0xFF, (float)m_diffuseColor.a / 0xFF};
    float specularColor[4] = {(float)m_specularColor.r / 0xFF, (float)m_specularColor.g / 0xFF, (float)m_specularColor.b / 0xFF, (float)m_specularColor.a / 0xFF};

    GLuint currProgram = pState->GetShaderProgram();
    if ( currProgram != 0 )
    {
        GLint ambientLocation = CRenderer::GetInstance()->GetUniformLocation( currProgram, COMMON_UNIFORM_NAMES[UNIFORM_AMBIENT_COLOR]);
        if ( -1 != ambientLocation )
        {
            CRenderer::GetInstance()->SetUniform1fv( ambientLocation, ambientColor, 4 );
        }
        GLint diffuseLocation = CRenderer::GetInstance()->GetUniformLocation( currProgram, COMMON_UNIFORM_NAMES[UNIFORM_DIFFUSE_COLOR]);
        if ( -1 != diffuseLocation )
        {
            CRenderer::GetInstance()->SetUniform1fv( diffuseLocation, diffuseColor, 4 );
        }
        GLint specularLocation = CRenderer::GetInstance()->GetUniformLocation( currProgram, COMMON_UNIFORM_NAMES[UNIFORM_SPECULAR_COLOR]);
        if ( -1 != specularLocation )
        {
            CRenderer::GetInstance()->SetUniform1fv( specularLocation, specularColor, 4 );
        }
        GLint shininessLocation = CRenderer::GetInstance()->GetUniformLocation( currProgram, COMMON_UNIFORM_NAMES[UNIFORM_SHININESS]);
        if ( -1 != shininessLocation )
        {
            CRenderer::GetInstance()->SetUniform1fv( shininessLocation, &m_fShininess, 1 );
        }
    }
}

CRenderState* CMaterial::GetRenderState() const
{
    return m_pRenderState;
}

void CMaterial::AddTexture( const TString& fileName )
{
    SharePtr< CTexture > pTextrue( 
        CResourceManager::GetInstance()->GetResource< CTexture >( fileName, false));
    m_textures.push_back( pTextrue );
}

void CMaterial::SetTexture(size_t stage, SharePtr<CTexture> texture)
{
    if(stage >= m_textures.size())
    {
        m_textures.resize(stage + 1);
    }
    m_textures[stage] = texture;
}

void CMaterial::SetAmbientColor( const CColor& color )
{
    m_ambientColor = color;
}

void CMaterial::SetDiffuseColor( const CColor& color )
{
    m_diffuseColor = color;
}

void CMaterial::SetSpecularColor( const CColor& color )
{
    m_specularColor = color;
}

void CMaterial::SetShininess( float shininess )
{
    m_fShininess = shininess;
}

bool CMaterial::operator==( const CMaterial& other ) const
{
    bool bEqual = false;
    if ( CompareVector( m_textures, other.m_textures ) && 
        ComparePtrVector( m_uniforms, other.m_uniforms ) )
    {
        if ( m_ambientColor == other.m_ambientColor &&
            m_diffuseColor == other.m_diffuseColor &&
            m_specularColor == other.m_specularColor &&
            m_fShininess == other.m_fShininess )
        {
            if ( *m_pRenderState == *other.m_pRenderState )
            {
                bEqual = true;
            }
        }
    }
    return bEqual;
}

bool CMaterial::operator!=( const CMaterial& other) const
{
    return !((*this) == other);
}

void CMaterial::AddUniform( CShaderUniform* uniform )
{
    m_uniforms.push_back( uniform );
}

bool CMaterial::CompareVector( const std::vector< SharePtr<CTexture>>& v1, const std::vector< SharePtr<CTexture>>& v2 ) const
{
    bool bReturn = true;
    if( v1 != v2 )
    {
        if ( v1.size() == v2.size() )
        {
            size_t size = v1.size();
            for ( size_t i = 0; i < size; ++i )
            {
                if ( v1[ i ].Get() != v2[ i ].Get() )
                {
                    if ( *v1[ i ].Get() != *v2[ i ].Get())
                    {
                        bReturn = false;
                        break;
                    }
                }
            }
        }
        else
        {
            bReturn = false;
        }
    }
    return bReturn;
}
