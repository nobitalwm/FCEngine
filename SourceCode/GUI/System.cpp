#include "stdafx.h"
#include "System.h"
#include "Renderer/BaseRenderer.h"
#include "Window/Window.h"
#include "Render/RenderManager.h"
#include "Event/MouseEvent.h"
#include "WindowManager.h"
#include "Render/Camera.h"
#include "GUI/Font/FontManager.h"
#include "Render/Material.h"
#include "Render/RenderState.h"
#include "Render/ShaderProgram.h"
#include "Resource/ResourceManager.h"
#include "Render/Shader.h"
#include "Render/RenderGroupManager.h"
#include "Render/QuadBatchRenderGroup.h"

using namespace FCGUI;

CSystem *CSystem::m_pInstance = nullptr;

CSystem::CSystem()
    : m_pRenderGroup(nullptr)
    , m_pProgram(nullptr)
    , m_pCamera(nullptr)
{
    kmVec2Fill(&m_resolution, 0.f, 0.f);
}

CSystem::~CSystem()
{
    BEATS_SAFE_DELETE(m_pCamera);
}

void CSystem::Init()
{
    InitFontFace();
    if(CWindowManager::GetInstance()->RootWindow())
    {
        CWindowManager::GetInstance()->RootWindow()->SetPos(0.f, 0.f);
        CWindowManager::GetInstance()->RootWindow()->SetSize(m_resolution);
    }

    m_pCamera = new CCamera(CCamera::eCT_2D);
    int width, height;
    CRenderManager::GetInstance()->GetWindowSize(width, height);
    m_pCamera->SetWidth(width);
    m_pCamera->SetHeight(height);
}

void CSystem::Update(float deltaTime)
{
    CWindowManager::GetInstance()->Update(deltaTime);
}

void CSystem::PreRender()
{
}

void CSystem::Render()
{
    CBaseRenderer *renderer = CWindowManager::GetInstance()->RootWindow()->Renderer();
    if(renderer)
    {
        kmMat4 transform;
        kmMat4Identity(&transform);

        PreRender();
        renderer->Render(transform);
        PostRender();
    }
}

void CSystem::PostRender()
{

}

void CSystem::OnResolutionChanged( kmVec2 resolution )
{
    m_resolution = resolution;
    if(CWindowManager::GetInstance()->RootWindow())
    {
        CWindowManager::GetInstance()->RootWindow()->SetPos(0.f, 0.f);
        CWindowManager::GetInstance()->RootWindow()->SetSize(resolution);
    }
}

void CSystem::OnResolutionChanged(kmScalar width, kmScalar height)
{
    kmVec2 resolution;
    kmVec2Fill(&resolution, width, height);
    OnResolutionChanged(resolution);
}

kmVec2 CSystem::GetResolution() const
{
    return m_resolution;
}

bool CSystem::InjectMouseEvent( CMouseEvent *event)
{
    return CWindowManager::GetInstance()->OnMouseEvent(event);
}

bool CSystem::InjectKeyboardEvent( CKeyboardEvent *event)
{
    return CWindowManager::GetInstance()->OnKeyboardEvent(event);
}

void CSystem::InitFontFace()
{
    CFontManager::GetInstance()->CreateFace(_T("STFANGSO_12"), _T("STFANGSO.TTF"), 24);
}

SharePtr<CMaterial> CSystem::CreateDefaultMaterial()
{
    if(!m_pProgram)
    {
        InitShader();
    }
    BEATS_ASSERT(m_pProgram);
    SharePtr<CMaterial> material = new CMaterial(true);
    material->GetRenderState()->SetBoolState(GL_DEPTH_TEST, false);
    material->GetRenderState()->SetBoolState(GL_BLEND, true);
    material->GetRenderState()->SetBlendFuncSrcFactor(GL_SRC_ALPHA);
    material->GetRenderState()->SetBlendFuncTargetFactor(GL_ONE_MINUS_SRC_ALPHA);
    material->GetRenderState()->SetShaderProgram(m_pProgram->ID());
    return material;
}

CQuadBatchRenderGroup *CSystem::GetRenderGroup()
{
    if(!m_pRenderGroup)
    {
        m_pRenderGroup = dynamic_cast<CQuadBatchRenderGroup *>(
            CRenderGroupManager::GetInstance()->GetRenderGroup(CRenderGroupManager::LAYER_GUI));
        BEATS_ASSERT(m_pRenderGroup);
    }
    return m_pRenderGroup;
}

CCamera *CSystem::GetCamera()
{
    return m_pCamera;
}

void CSystem::InitShader()
{
    SharePtr<CShader> pVS = CResourceManager::GetInstance()->GetResource<CShader>(_T("PointTexShader.vs"), false);
    SharePtr<CShader> pPS = CResourceManager::GetInstance()->GetResource<CShader>(_T("PointTexShader.ps"), false);
    BEATS_ASSERT(pVS && pPS, _T("Load Shader Failed!"));
    m_pProgram = CRenderManager::GetInstance()->GetShaderProgram(pVS->ID(), pPS->ID());
}
