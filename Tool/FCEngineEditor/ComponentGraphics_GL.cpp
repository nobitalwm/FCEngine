#include "stdafx.h"
#include "ComponentGraphics_GL.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentEditorProxy.h"
#include "Utility/BeatsUtility/ComponentSystem/Dependency/DependencyDescription.h"
#include "Utility/BeatsUtility/ComponentSystem/Dependency/DependencyDescriptionLine.h"
#include "utility/BeatsUtility/Serializer.h"
#include "Render/Texture.h"
#include "Render/Material.h"
#include "render/RenderManager.h"
#include "render/Renderer.h"
#include "GUI/Font/FontManager.h"
#include "GUI/Font/FontFace.h"
#include "Resource/ResourceManager.h"
#include "Render/Shader.h"
#include "Render/ShaderProgram.h"
#include "Render/RenderGroupManager.h"
#include "Render/QuadBatchRenderGroup.h"

static const float Font_Render_X_Offset = 8.0f;

enum ERenderLayer
{
    eRL_BackGround,
    eRL_Component,
    eRL_Dependency,
    eRL_SelectRect,

    eRL_Count,
    eRL_Force32Bit = 0xFFFFFFFF
};

CComponentGraphic_GL::CComponentGraphic_GL()
    : m_pFont(NULL)
{
    // Base class is dx implemented.
    m_gridPosZ *= -1;
}

CComponentGraphic_GL::~CComponentGraphic_GL()
{

}

void CComponentGraphic_GL::CreateMaterials()
{
    if (m_pFont == NULL)
    {
        m_pFont = FCGUI::CFontManager::GetInstance()->GetFace(_T("ComponentGraphicFont"));
        if (m_pFont == NULL)
        {
            m_pFont = FCGUI::CFontManager::GetInstance()->CreateFace(_T("ComponentGraphicFont"), _T("msyh.ttf"), 12, 0);
        }
    }
    BEATS_ASSERT(m_pFont != NULL);
    if (m_pMaterials[eCT_RectBG].Get() == NULL)
    {
        m_pMaterials[eCT_RectBG] = CreateMaterial(_T("Component/RectBG.jpg"));
        m_pMaterials[eCT_ConnectRect] = CreateMaterial(_T("Component/ConnectRect.jpg"));
        m_pMaterials[eCT_SelectedRectBG] = CreateMaterial(_T("Component/SelectedRect.png"));
        m_pMaterials[eCT_NormalLine] = CreateMaterial(_T("Component/NormalLine.jpg"));
        m_pMaterials[eCT_SelectedLine] = CreateMaterial(_T("Component/SelectedLine.jpg"));
        m_pMaterials[eCT_NormalArrow] = CreateMaterial(_T("Component/NormalArrow.png"));
        m_pMaterials[eCT_SelectedArrow] = CreateMaterial(_T("Component/SelectedArrow.png"));
        m_pMaterials[eCT_ConnectedDependency] = CreateMaterial(_T("Component/ConnectedDependency.jpg"));
        m_pMaterials[eCT_ConnectedDependencyList] = CreateMaterial(_T("Component/ConnectedDependencyList.jpg"));
        m_pMaterials[eCT_WeakDependency] = CreateMaterial(_T("Component/WeakDependency.jpg"));
        m_pMaterials[eCT_WeakDependencyList] = CreateMaterial(_T("Component/WeakDependencyList.jpg"));
        m_pMaterials[eCT_StrongDependency] = CreateMaterial(_T("Component/StrongDependency.jpg"));
        m_pMaterials[eCT_StrongDependencyList] = CreateMaterial(_T("Component/StrongDependencyList.jpg"));
    }
}

SharePtr<CMaterial> CComponentGraphic_GL::CreateMaterial(const TString &textureFileName)
{
    SharePtr<CMaterial> material = new CMaterial(true);
    material->GetRenderState()->SetBoolState(GL_DEPTH_TEST, true);
    material->GetRenderState()->SetBoolState(GL_ALPHA_TEST, true);
    material->GetRenderState()->SetAlphaFunc(GL_GREATER);
    material->GetRenderState()->SetAlphaRef(0);
    material->GetRenderState()->SetBoolState(GL_BLEND, true);
    material->GetRenderState()->SetBlendFuncSrcFactor(GL_SRC_ALPHA);
    material->GetRenderState()->SetBlendFuncTargetFactor(GL_ONE_MINUS_SRC_ALPHA);
    SharePtr<CShader> pVS = CResourceManager::GetInstance()->GetResource<CShader>(_T("PointTexShader.vs"), false);
    SharePtr<CShader> pPS = CResourceManager::GetInstance()->GetResource<CShader>(_T("PointTexShader.ps"), false);
    BEATS_ASSERT(pVS && pPS, _T("Load Shader Failed!"));
    CShaderProgram *program = CRenderManager::GetInstance()->GetShaderProgram(pVS->ID(), pPS->ID());
    material->GetRenderState()->SetShaderProgram(program->ID());
    SharePtr<CTexture> texture = CResourceManager::GetInstance()->GetResource<CTexture>(textureFileName, false);
    BEATS_ASSERT(texture, _T("Create Component Texture Failed!"));
    material->SetTexture(0, texture);
    return material;
}

void CComponentGraphic_GL::GetDependencyPosition(size_t uDependencyIndex, int* pOutX, int* pOutY)
{
    BEATS_ASSERT(m_pOwner->GetDependency(uDependencyIndex) != NULL);
    *pOutX = m_gridPosX + MIN_WIDTH;
    *pOutY = m_gridPosY + HEADER_HEIGHT + (int)(uDependencyIndex + 1) * DEPENDENCY_HEIGHT;
}

CComponentGraphic* CComponentGraphic_GL::Clone()
{
    return new CComponentGraphic_GL();
}

void CComponentGraphic_GL::DrawHead(float cellSize)
{
    CreateMaterials();

    CQuadBatchRenderGroup *renderGroup = dynamic_cast<CQuadBatchRenderGroup *>(
        CRenderGroupManager::GetInstance()->GetRenderGroup(CRenderGroupManager::LAYER_GUI));
    BEATS_ASSERT(renderGroup);

    CQuadPTC head;
    head.tl.position.x = (m_gridPosX + CONNECTION_WIDTH) * cellSize;// Left top
    head.tl.position.y = m_gridPosY * cellSize;
    head.tl.position.z = (float)m_gridPosZ;
    head.tl.tex.u = 0;
    head.tl.tex.v = 0;

    head.bl.position.x = (m_gridPosX + CONNECTION_WIDTH) * cellSize;// Left down
    head.bl.position.y = (m_gridPosY + HEADER_HEIGHT) * cellSize;
    head.bl.position.z = (float)m_gridPosZ;
    head.bl.tex.u = 0;
    head.bl.tex.v = 1;

    head.tr.position.x = (m_gridPosX + CONNECTION_WIDTH + MIN_WIDTH) * cellSize;    // Right top
    head.tr.position.y = m_gridPosY * cellSize;
    head.tr.position.z = (float)m_gridPosZ;
    head.tr.tex.u = 1;
    head.tr.tex.v = 0;

    head.br.position.x = (m_gridPosX + CONNECTION_WIDTH + MIN_WIDTH) * cellSize;    // Right down
    head.br.position.y = (m_gridPosY + HEADER_HEIGHT) * cellSize;
    head.br.position.z = (float)m_gridPosZ;
    head.br.tex.u = 1;
    head.br.tex.v = 1;
    renderGroup->AddQuad(head, m_pMaterials[eCT_RectBG]);

    // 2. Draw title text.
    m_pFont->RenderText(m_pOwner->GetDisplayName(), head.tl.position.x + Font_Render_X_Offset, head.tl.position.y, 0xFFFFFFFF);

    // 3. Draw Connect rect.
    CQuadPTC connectRect;

    connectRect.tl.position.x = m_gridPosX * cellSize;    // Left top
    connectRect.tl.position.y = m_gridPosY * cellSize;
    connectRect.tl.position.z = (float)m_gridPosZ;
    connectRect.tl.tex.u = 0;
    connectRect.tl.tex.v = 0;

    connectRect.bl.position.x = m_gridPosX * cellSize;    // Left down
    connectRect.bl.position.y = (m_gridPosY + HEADER_HEIGHT) * cellSize;
    connectRect.bl.position.z = (float)m_gridPosZ;
    connectRect.bl.tex.u = 0;
    connectRect.bl.tex.v = 1;

    connectRect.tr.position.x = (m_gridPosX + CONNECTION_WIDTH) * cellSize;    // Right top
    connectRect.tr.position.y = m_gridPosY * cellSize;
    connectRect.tr.position.z = (float)m_gridPosZ;    
    connectRect.tr.tex.u = 1;
    connectRect.tr.tex.v = 0;

    connectRect.br.position.x = (m_gridPosX + CONNECTION_WIDTH) * cellSize;    // Right down
    connectRect.br.position.y = (m_gridPosY + HEADER_HEIGHT) * cellSize;
    connectRect.br.position.z = (float)m_gridPosZ;    
    connectRect.br.tex.u = 1;
    connectRect.br.tex.v = 1;

    renderGroup->AddQuad(connectRect, m_pMaterials[eCT_ConnectRect]);
}

void CComponentGraphic_GL::DrawDependencies( float cellSize )
{
    CreateMaterials();

    CQuadBatchRenderGroup *renderGroup = dynamic_cast<CQuadBatchRenderGroup *>(
        CRenderGroupManager::GetInstance()->GetRenderGroup(CRenderGroupManager::LAYER_GUI));
    BEATS_ASSERT(renderGroup);

    size_t uDependencyCount = m_pOwner->GetDependencies().size();
    for (int i = 0; i < (int)uDependencyCount; ++i)
    {
        // 1. Draw background.
        CQuadPTC dependencyPoint;
        dependencyPoint.tl.position.x = m_gridPosX * cellSize;    // Left top
        dependencyPoint.tl.position.y = (m_gridPosY + HEADER_HEIGHT + i * DEPENDENCY_HEIGHT) * cellSize;
        dependencyPoint.tl.position.z = (float)m_gridPosZ;
        dependencyPoint.tl.tex.u = 0;
        dependencyPoint.tl.tex.v = 0;

        dependencyPoint.bl.position.x = m_gridPosX * cellSize;    // Left down
        dependencyPoint.bl.position.y = (m_gridPosY + HEADER_HEIGHT + (i + 1) * DEPENDENCY_HEIGHT) * cellSize;
        dependencyPoint.bl.position.z = (float)m_gridPosZ;
        dependencyPoint.bl.tex.u = 0;
        dependencyPoint.bl.tex.v = 1;

        dependencyPoint.tr.position.x = (m_gridPosX + MIN_WIDTH) * cellSize;    // Right top
        dependencyPoint.tr.position.y = (m_gridPosY + HEADER_HEIGHT + i * DEPENDENCY_HEIGHT) * cellSize ;
        dependencyPoint.tr.position.z = (float)m_gridPosZ;
        dependencyPoint.tr.tex.u = 1;
        dependencyPoint.tr.tex.v = 0;

        dependencyPoint.br.position.x = (m_gridPosX + MIN_WIDTH) * cellSize;    // Right down
        dependencyPoint.br.position.y = (m_gridPosY + HEADER_HEIGHT + (i + 1) * DEPENDENCY_HEIGHT) * cellSize;
        dependencyPoint.br.position.z = (float)m_gridPosZ;
        dependencyPoint.br.tex.u = 1;
        dependencyPoint.br.tex.v = 1;

        renderGroup->AddQuad(dependencyPoint, m_pMaterials[eCT_RectBG]);

        // 2. Draw dependency name.
        m_pFont->RenderText(m_pOwner->GetDependency(i)->GetDisplayName(), dependencyPoint.tl.position.x + Font_Render_X_Offset, dependencyPoint.tl.position.y, 0xFFFFFFFF);

        // 3. Draw Connect rect.
        CDependencyDescription* pDescription = m_pOwner->GetDependency(i);
        BEATS_ASSERT(pDescription != NULL);
        bool bConnected = pDescription->GetDependencyLineCount() > 0;
        EComponentTexture textureType = eCT_Count;
        EDependencyType descriptionType = pDescription->GetType();
        bool bIsList = pDescription->IsListType();
        if (bConnected)
        {
            textureType = bIsList ? eCT_ConnectedDependencyList : eCT_ConnectedDependency;
        }
        else
        {
            if (bIsList)
            {
                textureType = descriptionType == eDT_Strong ? eCT_StrongDependencyList : eCT_WeakDependencyList;
            }
            else
            {
                textureType = descriptionType == eDT_Strong ? eCT_StrongDependency : eCT_WeakDependency;
            }
        }
        BEATS_ASSERT(textureType != eCT_Count);

        dependencyPoint.tl.position.x = (m_gridPosX + MIN_WIDTH) * cellSize;    // Left top
        dependencyPoint.tl.position.y = (m_gridPosY + HEADER_HEIGHT + i * DEPENDENCY_HEIGHT) * cellSize;
        dependencyPoint.tl.position.z = (float)m_gridPosZ;
        dependencyPoint.tl.tex.u = 0;
        dependencyPoint.tl.tex.v = 0;

        dependencyPoint.bl.position.x = (m_gridPosX + MIN_WIDTH) * cellSize;    // Left down
        dependencyPoint.bl.position.y = (m_gridPosY + HEADER_HEIGHT + (i + 1) * DEPENDENCY_HEIGHT) * cellSize;
        dependencyPoint.bl.position.z = (float)m_gridPosZ;
        dependencyPoint.bl.tex.u = 0;
        dependencyPoint.bl.tex.v = 1;

        dependencyPoint.tr.position.x = (m_gridPosX + MIN_WIDTH + CONNECTION_WIDTH) * cellSize;    // Right top
        dependencyPoint.tr.position.y = (m_gridPosY + HEADER_HEIGHT + i * DEPENDENCY_HEIGHT) * cellSize;
        dependencyPoint.tr.position.z = (float)m_gridPosZ;
        dependencyPoint.tr.tex.u = 1;
        dependencyPoint.tr.tex.v = 0;

        dependencyPoint.br.position.x = (m_gridPosX + MIN_WIDTH + CONNECTION_WIDTH) * cellSize;    // Right down
        dependencyPoint.br.position.y = (m_gridPosY + HEADER_HEIGHT + (i + 1) * DEPENDENCY_HEIGHT) * cellSize;
        dependencyPoint.br.position.z = (float)m_gridPosZ;
        dependencyPoint.br.tex.u = 1;
        dependencyPoint.br.tex.v = 1;

        renderGroup->AddQuad(dependencyPoint, m_pMaterials[textureType]);

        // 4. Draw the line.
        DrawDependencyLine(cellSize, m_pOwner->GetDependency(i));
    }
}

void CComponentGraphic_GL::DrawDependencyLine( float /*cellSize*/, const CDependencyDescription* pDependency )
{
    CreateMaterials();

    CQuadBatchRenderGroup *renderGroup = dynamic_cast<CQuadBatchRenderGroup *>(
        CRenderGroupManager::GetInstance()->GetRenderGroup(CRenderGroupManager::LAYER_GUI));
    BEATS_ASSERT(renderGroup);

    BEATS_ASSERT(pDependency != NULL);
    if (pDependency->IsVisible())
    {
        size_t uDependencyLineCount = pDependency->GetDependencyLineCount();
        for (size_t i = 0; i < uDependencyLineCount; ++i)
        {
            CDependencyDescriptionLine* pDependencyLine = pDependency->GetDependencyLine(i);
            const SVertex* pData = pDependencyLine->GetRectArray();
            static const size_t SVERTEX_SIZE = 24;
            CSerializer serializer(SVERTEX_SIZE * 4, (void*)pData);
            DWORD tmpColor = 0;
            CQuadPTC dependencyLine;
            serializer >> dependencyLine.br.position.x;
            serializer >> dependencyLine.br.position.y;
            serializer >> dependencyLine.br.position.z;
            serializer >> tmpColor;
            serializer >> dependencyLine.br.tex.u;
            serializer >> dependencyLine.br.tex.v;

            serializer >> dependencyLine.bl.position.x;
            serializer >> dependencyLine.bl.position.y;
            serializer >> dependencyLine.bl.position.z;
            serializer >> tmpColor;
            serializer >> dependencyLine.bl.tex.u;
            serializer >> dependencyLine.bl.tex.v;

            serializer >> dependencyLine.tr.position.x;
            serializer >> dependencyLine.tr.position.y;
            serializer >> dependencyLine.tr.position.z;
            serializer >> tmpColor;
            serializer >> dependencyLine.tr.tex.u;
            serializer >> dependencyLine.tr.tex.v;

            serializer >> dependencyLine.tl.position.x;
            serializer >> dependencyLine.tl.position.y;
            serializer >> dependencyLine.tl.position.z;
            serializer >> tmpColor;
            serializer >> dependencyLine.tl.tex.u;
            serializer >> dependencyLine.tl.tex.v;

            renderGroup->AddQuad(dependencyLine, m_pMaterials[pDependencyLine->IsSelected() ? eCT_SelectedLine : eCT_NormalLine]);

            const SVertex* pArrowData = pDependencyLine->GetArrowRectArray();
            CSerializer serializerArrow(SVERTEX_SIZE * 4, (void*)pArrowData);
            CQuadPTC arrow;
            serializerArrow >> arrow.br.position.x;
            serializerArrow >> arrow.br.position.y;
            serializerArrow >> arrow.br.position.z;
            serializerArrow >> tmpColor;
            serializerArrow >> arrow.br.tex.u;
            serializerArrow >> arrow.br.tex.v;

            serializerArrow >> arrow.bl.position.x;
            serializerArrow >> arrow.bl.position.y;
            serializerArrow >> arrow.bl.position.z;
            serializerArrow >> tmpColor;
            serializerArrow >> arrow.bl.tex.u;
            serializerArrow >> arrow.bl.tex.v;

            serializerArrow >> arrow.tr.position.x;
            serializerArrow >> arrow.tr.position.y;
            serializerArrow >> arrow.tr.position.z;
            serializerArrow >> tmpColor;
            serializerArrow >> arrow.tr.tex.u;
            serializerArrow >> arrow.tr.tex.v;

            serializerArrow >> arrow.tl.position.x;
            serializerArrow >> arrow.tl.position.y;
            serializerArrow >> arrow.tl.position.z;
            serializerArrow >> tmpColor;
            serializerArrow >> arrow.tl.tex.u;
            serializerArrow >> arrow.tl.tex.v;

            renderGroup->AddQuad(arrow, m_pMaterials[pDependencyLine->IsSelected() ? eCT_SelectedArrow : eCT_NormalArrow]);

            //Render index number for dependency list.
            if (pDependency->IsListType())
            {
                kmVec3 deltaDirection;
                kmVec3Subtract(&deltaDirection, &dependencyLine.tr.position, &dependencyLine.tl.position);
                float fXPos = (dependencyLine.tl.position.x + deltaDirection.x * 0.15f);
                static const float fHardCodeOffset = 8;
                float fYPos = (dependencyLine.tl.position.y + deltaDirection.y * 0.15f - fHardCodeOffset);

                TCHAR szIndex[8];
                _stprintf(szIndex, _T("%d"), pDependencyLine->GetIndex());
                m_pFont->RenderText(szIndex, fXPos, fYPos, 0xffff00ff);
            }
        }
    }
}

void CComponentGraphic_GL::DrawSelectedRect(float fCellSize)
{
    CreateMaterials();

    CQuadBatchRenderGroup *renderGroup = dynamic_cast<CQuadBatchRenderGroup *>(
        CRenderGroupManager::GetInstance()->GetRenderGroup(CRenderGroupManager::LAYER_GUI));
    BEATS_ASSERT(renderGroup);

    float fWidth = GetWidth() * fCellSize;
    float fHeight = GetHeight() * fCellSize;
    int x, y;
    GetPosition(&x, &y);
    CQuadPTC rect;
    // left top
    rect.tl.position.x = x * fCellSize;
    rect.tl.position.y = y * fCellSize;
    rect.tl.position.z = (float)m_gridPosZ + eRL_SelectRect;
    rect.tl.tex.u = 0;
    rect.tl.tex.v = 0;
    // left down
    rect.bl.position.x = x * fCellSize;
    rect.bl.position.y = y * fCellSize + fHeight;
    rect.bl.position.z = (float)m_gridPosZ + eRL_SelectRect;
    rect.bl.tex.u = 0;
    rect.bl.tex.v = 1;
    // right top
    rect.tr.position.x = x * fCellSize + fWidth;
    rect.tr.position.y = y * fCellSize;
    rect.tr.position.z = (float)m_gridPosZ + eRL_SelectRect;
    rect.tr.tex.u = 1;
    rect.tr.tex.v = 0;
    // right down
    rect.br.position.x = x * fCellSize + fWidth;
    rect.br.position.y = y * fCellSize + fHeight;
    rect.br.position.z = (float)m_gridPosZ + eRL_SelectRect;
    rect.br.tex.u = 1;
    rect.br.tex.v = 1;
    renderGroup->AddQuad(rect, m_pMaterials[eCT_SelectedRectBG]);
}

EComponentAeraRectType CComponentGraphic_GL::HitTestForAreaType( int x, int y, void** pReturnData)
{
    EComponentAeraRectType result = eCART_Invalid;
    if (x >= m_gridPosX && x < m_gridPosX + (int)m_width && y > m_gridPosY && y <= m_gridPosY + (int)m_height)
    {
        result = eCART_Body;
        if (y <= m_gridPosY + HEADER_HEIGHT && x < m_gridPosX + CONNECTION_WIDTH)
        {
            result = eCART_Connection;
        }
        else if (x >= m_gridPosX + MIN_WIDTH && y > m_gridPosY + HEADER_HEIGHT)
        {
            result = eCART_Dependency;
            if (pReturnData != NULL)
            {
                (*pReturnData) = m_pOwner->GetDependency((y - m_gridPosY - HEADER_HEIGHT - 1) / DEPENDENCY_HEIGHT);
            }
        }
    }
    return result;
}