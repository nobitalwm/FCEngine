#include "stdafx.h"
#include "QuadBatch.h"
#include "RenderManager.h"
#include "Texture.h"
#include "Renderer.h"
#include "Material.h"

CQuadBatch::CQuadBatch(SharePtr<CMaterial> material)
    : m_pMaterial(material)
    , m_uCount(0)
    , m_VAO(0)
{
    memset(m_quads, 0, sizeof(m_quads));
    CRenderer::GetInstance()->GenVertexArrays(1, &m_VAO);
    CRenderer::GetInstance()->GenBuffers(2, m_VBO);
    setupVAO();
}

CQuadBatch::~CQuadBatch()
{
    CRenderer::GetInstance()->DeleteVertexArrays(1, &m_VAO);
    CRenderer::GetInstance()->DeleteBuffers(2, m_VBO);
}

void CQuadBatch::PreRender()
{
    if(m_uCount > 0)
    {
        updateVBO();
        CRenderer::GetInstance()->BindVertexArray(m_VAO);
        m_pMaterial->Use();
    }
}

void CQuadBatch::Render()
{
    if(m_uCount > 0)
    {
        CRenderer::GetInstance()->DrawElements(
            GL_TRIANGLES, m_uCount*6, GL_UNSIGNED_SHORT, 0);
    }
}

void CQuadBatch::PostRender()
{
    if(m_uCount > 0)
    {
        CRenderer::GetInstance()->BindVertexArray(0);
    }
}

void CQuadBatch::AddQuad(const CQuadP &quadP, const CQuadT &quadT, const kmMat4 &transform)
{
    CQuadPTC &quadPTC = m_quads[m_uCount];
    kmVec3Transform(&quadPTC.tl.position, &quadP.tl, &transform);
    kmVec3Transform(&quadPTC.tr.position, &quadP.tr, &transform);
    kmVec3Transform(&quadPTC.bl.position, &quadP.bl, &transform);
    kmVec3Transform(&quadPTC.br.position, &quadP.br, &transform);
    quadPTC.tl.tex = quadT.tl;
    quadPTC.tr.tex = quadT.tr;
    quadPTC.bl.tex = quadT.bl;
    quadPTC.br.tex = quadT.br;
    ++m_uCount;
}

void CQuadBatch::AddQuad( const CQuadPTC &quad)
{
    m_quads[m_uCount++] = quad;
}

void CQuadBatch::setupVAO()
{
    CRenderer *pRenderer = CRenderer::GetInstance();
    pRenderer->BindVertexArray(m_VAO);

    pRenderer->BindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);

    pRenderer->EnableVertexAttribArray(ATTRIB_INDEX_POSITION);
    pRenderer->EnableVertexAttribArray(ATTRIB_INDEX_TEXCOORD0);
    pRenderer->EnableVertexAttribArray(ATTRIB_INDEX_COLOR);

    pRenderer->VertexAttribPointer(ATTRIB_INDEX_POSITION, 
        3, GL_FLOAT, GL_FALSE, sizeof(CVertexPTC), (const GLvoid *)offsetof(CVertexPTC, position));
    pRenderer->VertexAttribPointer(ATTRIB_INDEX_TEXCOORD0,
        2, GL_FLOAT, GL_FALSE, sizeof(CVertexPTC), (const GLvoid *)offsetof(CVertexPTC, tex));
    pRenderer->VertexAttribPointer(ATTRIB_INDEX_COLOR,
        4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(CVertexPTC), (const GLvoid *)offsetof(CVertexPTC, color));

    pRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[1]);

    pRenderer->BindVertexArray(0);
    pRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    pRenderer->BindBuffer(GL_ARRAY_BUFFER, 0);
}

void CQuadBatch::updateVBO()
{
    CRenderer *pRenderer = CRenderer::GetInstance();

    pRenderer->BindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
    pRenderer->BufferData(GL_ARRAY_BUFFER, m_uCount*sizeof(CQuadPTC), m_quads, GL_DYNAMIC_DRAW);

    pRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[1]);
    pRenderer->BufferData(GL_ELEMENT_ARRAY_BUFFER, m_uCount * 12, nullptr, GL_DYNAMIC_DRAW);
    GLushort *pIndex = (GLushort *)pRenderer->MapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
    GLushort *pIndexWritter = pIndex;
    for(size_t i = 0; i < m_uCount; ++i)
    {
        GLushort start = static_cast<GLushort>(i*4);
        *(pIndexWritter++) = start;
        *(pIndexWritter++) = start + 1;
        *(pIndexWritter++) = start + 2;
        *(pIndexWritter++) = start + 3;
        *(pIndexWritter++) = start + 2;
        *(pIndexWritter++) = start + 1;
    }
    pRenderer->UnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void CQuadBatch::Clear()
{
    m_uCount = 0;
    memset(m_quads, 0, sizeof(m_quads));
}

SharePtr<CMaterial> CQuadBatch::GetMaterial() const
{
    return m_pMaterial;
}
