#include "stdafx.h"
#include "FontManager.h"
#include "FontFace.h"
#include "Render/Texture.h"
#include "Render/RenderManager.h"
#include "Render/Material.h"
#include "Render/QuadBatchRenderGroup.h"
#include "Render/Shader.h"
#include "Render/RenderState.h"
#include "Render/ShaderProgram.h"
#include "Render/RenderGroupManager.h"
#include "Resource/ResourceManager.h"

using namespace FCGUI;

CFontManager *CFontManager::m_pInstance = nullptr;

CFontManager::CFontManager()
    : m_pRenderGroup(nullptr)
    , m_pProgram(nullptr)
{

}

CFontManager::~CFontManager()
{
    for(auto face : m_faceMap)
    {
        BEATS_SAFE_DELETE(face.second);
    }
}

CFontFace *CFontManager::CreateFace( const TString &name, const TString &file, int sizeInPt, int dpi )
{
    auto itr = m_faceMap.find(name);
    BEATS_ASSERT(itr == m_faceMap.end(), _T("Font face:%s already existed"), name.c_str());

    CFontFace *face = new CFontFace(name, file, sizeInPt, dpi);
    m_faceMap[name] = face;
    return face;
}

CFontFace *CFontManager::GetFace( const TString &name )
{
    auto itr = m_faceMap.find(name);
    return itr != m_faceMap.end() ? itr->second : nullptr;
}

void CFontManager::RenderText(const TString &text, kmScalar x, kmScalar y, 
                             const TString &faceName, CColor color)
{
    CFontFace *face = GetFace(faceName);
    BEATS_ASSERT(face);

    face->RenderText(text, x, y, color);
}

SharePtr<CMaterial> CFontManager::CreateDefaultMaterial()
{
    if(!m_pProgram)
    {
        SharePtr<CShader> pVS = CResourceManager::GetInstance()->GetResource<CShader>(_T("TextShader.vs"), false);
        SharePtr<CShader> pPS = CResourceManager::GetInstance()->GetResource<CShader>(_T("TextShader.ps"), false);
        BEATS_ASSERT(pVS && pPS, _T("Load Shader Failed!"));
        m_pProgram = CRenderManager::GetInstance()->GetShaderProgram(pVS->ID(), pPS->ID());
    }
    SharePtr<CMaterial> material = new CMaterial(true);
    material->GetRenderState()->SetBoolState(GL_DEPTH_TEST, false);
    material->GetRenderState()->SetBoolState(GL_BLEND, true);
    material->GetRenderState()->SetBlendFuncSrcFactor(GL_SRC_ALPHA);
    material->GetRenderState()->SetBlendFuncTargetFactor(GL_ONE_MINUS_SRC_ALPHA);
    material->GetRenderState()->SetShaderProgram(m_pProgram->ID());

    return material;
}

CQuadBatchRenderGroup *CFontManager::GetRenderGroup()
{
    if(!m_pRenderGroup)
    {
        m_pRenderGroup = dynamic_cast<CQuadBatchRenderGroup *>(
            CRenderGroupManager::GetInstance()->GetRenderGroup(CRenderGroupManager::LAYER_GUI));
        BEATS_ASSERT(m_pRenderGroup);
    }
    return m_pRenderGroup;
}
