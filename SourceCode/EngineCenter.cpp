#include "stdafx.h"
#include "EngineCenter.h"
#include "Render/RenderObjectManager.h"
#include "Render/AnimationManager.h"
#include "Render/RenderManager.h"
#include "Resource/ResourceManager.h"
#include "Resource/ResourcePathManager.h"
#include "Render/renderer.h"
#include "Utility/BeatsUtility/ComponentSystem/ComponentPublic.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentManager.h"
#include "Utility/BeatsUtility/StringHelper.h"
#include "Utility/BeatsUtility/EnumStrGenerator.h"
#include "Utility/PerformDetector/PerformDetector.h"
#include "GUI/System.h"
#include "GUI/Animation/AnimationManager_ui.h"
#include "GUI/Font/FontManager.h"
#include "Render/TextureFragManager.h"
#include "Render/RenderGroupManager.h"
#include "GUI/WindowManager.h"
#include "LanguageManager.h"
#include "Render/TextureFormatConverter.h"
#include "ParticlesSystem/ParticleSystemManager.h"
#include "SkeletalAnimation/CAnimationManager.h"
#include "SkeletalAnimation/CAnimationImporter.h"

CEngineCenter* CEngineCenter::m_pInstance = NULL;

CEngineCenter::CEngineCenter()
    : m_uLastUpdateTime(0)
    , m_uFPS(60)
{

}

CEngineCenter::~CEngineCenter()
{
    CPerformDetector::Destroy();
    FCEngine::CParticleSystemManager::Destroy();
    _2DSkeletalAnimation::CAnimationImporter::Destroy();
    _2DSkeletalAnimation::CAnimationManager::Destroy();
    FCGUI::CAnimationManager::Destroy();
    FCGUI::CWindowManager::Destroy();
    FCGUI::CSystem::Destroy();
    FCGUI::CFontManager::Destroy();
    CRenderObjectManager::Destroy();
    CRenderGroupManager::Destroy();
    CTextureFragManager::Destroy();
    CRenderManager::Destroy();
    CResourcePathManager::Destroy();
    CComponentManager::Destroy();
    CEnumStrGenerator::Destroy();
    CStringHelper::Destroy();
    CResourceManager::Destroy();
    CAnimationManager::Destroy();
    CRenderer::Destroy();
    CLanguageManager::Destroy();
    CTextureFormatConverter::Destroy();
#ifdef _DEBUG
    SymCleanup(GetCurrentProcess());
#endif
}

bool CEngineCenter::Initialize(size_t uWindowWidth/* = 0xFFFFFFFF*/, size_t uWindowHeight/* = 0xFFFFFFFF*/)
{
#ifdef _DEBUG
    SymSetOptions(SYMOPT_LOAD_LINES);
    bool bInitializeSuccess = SymInitialize(GetCurrentProcess(), NULL, TRUE) == TRUE;
    BEATS_ASSERT(bInitializeSuccess);
#endif

    const TString& strSourceCodePath = CResourcePathManager::GetInstance()->GetResourcePath(CResourcePathManager::eRPT_SourceCode);
    std::vector<TString> scanPath;
    scanPath.push_back(strSourceCodePath);
    CEnumStrGenerator::GetInstance()->Init(scanPath);

    if (0xFFFFFFFF == uWindowWidth && 0xFFFFFFFF == uWindowHeight)
    {
        CRenderManager::GetInstance()->Initialize();
    }
    else
    {
        CRenderManager::GetInstance()->InitializeWithWindow(uWindowWidth, uWindowHeight);
    }
    FCGUI::CWindowManager::GetInstance()->Init();
    FCGUI::CSystem::GetInstance()->Init();
    pComponentLauncherFunc();
    CPerformDetector::GetInstance()->SetTypeName(pszPerformNodeStr, sizeof(pszPerformNodeStr));
    return true;
}

void CEngineCenter::Update()
{
    size_t uCurTime = GetTickCount();
    size_t uDeltaTime = uCurTime - m_uLastUpdateTime;
    if ( uDeltaTime >= (float)1000 / m_uFPS)
    {
        float dt = (float)uDeltaTime / 1000;
        CAnimationManager::GetInstance()->Update(dt);
        FCGUI::CAnimationManager::GetInstance()->Update(dt);
        _2DSkeletalAnimation::CAnimationManager::GetInstance()->Update(dt);
        FCEngine::CParticleSystemManager::GetInstance()->Update( dt );
        CRenderManager::GetInstance()->Render();
        m_uLastUpdateTime = uCurTime;
    }
}