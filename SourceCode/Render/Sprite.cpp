#include "stdafx.h"
#include "Sprite.h"
#include "Texture.h"
#include "RenderManager.h"
#include "TextureFragManager.h"
#include "TextureFrag.h"
#include "Material.h"
#include "RenderState.h"
#include "ShaderProgram.h"
#include "QuadBatchRenderGroup.h"
#include "RenderGroupManager.h"
#include "Shader.h"
#include "Resource/ResourceManager.h"

CSprite::CSprite(CTextureFrag *textureFrag)
    : m_textureFrag(textureFrag)
{
    kmVec2 origin;
    kmVec2Fill(&origin, 0.f, 0.f);
    SetOriginSize(origin, m_textureFrag->Size());
}

CSprite::CSprite(const TString &textureAtlasName, const TString &textureFragName)
    : m_textureFrag(CTextureFragManager::GetInstance()->GetTextureFrag(textureAtlasName, textureFragName))
{
    kmVec2 origin;
    kmVec2Fill(&origin, 0.f, 0.f);
    SetOriginSize(origin, m_textureFrag->Size());
}

CSprite::CSprite(const TString &textureAtlasName, const TString &textureFragName, const kmVec2 &size)
    : m_textureFrag(CTextureFragManager::GetInstance()->GetTextureFrag(textureAtlasName, textureFragName))
{
    kmVec2 origin;
    kmVec2Fill(&origin, 0.f, 0.f);
    SetOriginSize(origin, size);
}

CSprite::CSprite(const TString &textureAtlasName, const TString &textureFragName, 
                 const kmVec2 &size, const kmVec2 &origin)
    : m_textureFrag(CTextureFragManager::GetInstance()->GetTextureFrag(textureAtlasName, textureFragName))
{
    SetOriginSize(origin, size);
}

CSprite::~CSprite()
{
}

void CSprite::PreRender()
{
}

void CSprite::Render()
{
    kmMat4 transform;
    kmMat4Identity(&transform);

    BEATS_ASSERT(m_textureFrag);
    CQuadBatchRenderGroup *renderGroup = dynamic_cast<CQuadBatchRenderGroup *>(
        CRenderGroupManager::GetInstance()->GetRenderGroup(CRenderGroupManager::LAYER_2D));
    BEATS_ASSERT(renderGroup);
    renderGroup->AddQuad(m_quad, m_textureFrag->Quad(), transform, Material());
}

void CSprite::PostRender()
{
}

const CQuadP &CSprite::QuadP() const
{
    return m_quad;
}

const CQuadT &CSprite::QuadT() const
{
    return m_textureFrag->Quad();
}

SharePtr<CTexture> CSprite::Texture() const
{
    return m_textureFrag->Texture();
}

SharePtr<CMaterial> CSprite::Material()
{
    if(!m_material)
    {
        SharePtr<CShader> pVS = CResourceManager::GetInstance()->GetResource<CShader>(_T("PointTexShader.vs"), false);
        SharePtr<CShader> pPS = CResourceManager::GetInstance()->GetResource<CShader>(_T("PointTexShader.ps"), false);
        BEATS_ASSERT(pVS && pPS, _T("Load Shader Failed!"));
        CShaderProgram *program = CRenderManager::GetInstance()->GetShaderProgram(pVS->ID(), pPS->ID());
        m_material = new CMaterial(true);
        m_material->GetRenderState()->SetBoolState(GL_DEPTH_TEST, false);
        m_material->GetRenderState()->SetBoolState(GL_BLEND, true);
        m_material->GetRenderState()->SetBlendFuncSrcFactor(GL_SRC_ALPHA);
        m_material->GetRenderState()->SetBlendFuncTargetFactor(GL_ONE_MINUS_SRC_ALPHA);
        m_material->GetRenderState()->SetShaderProgram(program->ID());
        if(m_textureFrag)
            m_material->SetTexture(0, m_textureFrag->Texture());
    }
    return m_material;
}

void CSprite::SetQuad( const CQuadP &quad )
{
    m_quad = quad;
}

void CSprite::SetOriginSize( const kmVec2 &origin, const kmVec2 &size )
{
    //set vertex
    m_quad.tl.x = 0 - origin.x;
    m_quad.tl.y = 0 - origin.y;
    m_quad.tr.x = size.x - origin.x;
    m_quad.tr.y = m_quad.tl.y;
    m_quad.bl.x = m_quad.tl.x;
    m_quad.bl.y = size.y - origin.y;
    m_quad.br.x = m_quad.tr.x;
    m_quad.br.y = m_quad.bl.y;
}