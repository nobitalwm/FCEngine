#ifndef RENDER_MATERIAL_H__INCLUDE
#define RENDER_MATERIAL_H__INCLUDE

#include "resource/Resource.h"
#include "render/Texture.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"
#include "ShaderUniform.h"
class CRenderState;

class CMaterial : public CComponentBase, public CResource
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID(CMaterial, 0xD507AB1C, CComponentBase)
    DECLARE_RESOURCE_TYPE(eRT_Material)
public:
    CMaterial( bool bCreateRenderState = false );
    virtual ~CMaterial();

    virtual void ReflectData(CSerializer& serializer) override;
    virtual bool Load() override;
    virtual bool Unload() override;

    void Use();

    CRenderState* GetRenderState( ) const;

    void AddTexture( const TString& fileName );
    void SetTexture( size_t stage, SharePtr<CTexture> texture);

    void SetAmbientColor( const CColor& color );
    void SetDiffuseColor( const CColor& color );
    void SetSpecularColor( const CColor& color );
    void SetShininess( float shininess );

    bool operator==( const CMaterial& other ) const;
    bool operator!=( const CMaterial& other ) const;

    void AddUniform( CShaderUniform* uniform );
private:
    void SendUniform();

    void SendLightInfo( CRenderState* pState );

    bool CompareVector( const std::vector< SharePtr<CTexture>>& v1, const std::vector< SharePtr<CTexture>>& v2 ) const;

private:
    CRenderState* m_pRenderState;
    std::vector<SharePtr<CTexture>> m_textures;
    std::vector<CShaderUniform*> m_uniforms;

    CColor m_ambientColor;
    CColor m_diffuseColor;
    CColor m_specularColor;
    float m_fShininess;
};

#endif