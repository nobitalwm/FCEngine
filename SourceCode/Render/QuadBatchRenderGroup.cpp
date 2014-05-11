#include "stdafx.h"
#include "QuadBatchRenderGroup.h"
#include "QuadBatch.h"
#include "Material.h"

CQuadBatchRenderGroup::CQuadBatchRenderGroup(int ID)
    : CRenderGroup(ID)
{

}

CQuadBatchRenderGroup::~CQuadBatchRenderGroup()
{
    Clear();
}

void CQuadBatchRenderGroup::AddQuad(const CQuadP &quadP, const CQuadT &quadT, 
                                    const kmMat4 &transform, SharePtr<CMaterial> material)
{
    GetQuadBatch(material)->AddQuad(quadP, quadT, transform);
}

void CQuadBatchRenderGroup::AddQuad(const CQuadPTC &quad, SharePtr<CMaterial> material)
{
    GetQuadBatch(material)->AddQuad(quad);
}

void CQuadBatchRenderGroup::AddRenderObject(CRenderObject *object)
{
    BEATS_ASSERT(dynamic_cast<CQuadBatch *>(object));
    CRenderGroup::AddRenderObject(object);
}

CQuadBatch *CQuadBatchRenderGroup::GetQuadBatch(SharePtr<CMaterial> material)
{
    CQuadBatch *quadBatch = nullptr;
    if(m_objects.empty())
    {
        quadBatch = new CQuadBatch(material);
        AddRenderObject(quadBatch);
    }
    else
    {
        quadBatch = static_cast<CQuadBatch *>(m_objects.back());
        if(*material != *quadBatch->GetMaterial())
        {
            quadBatch = new CQuadBatch(material);
            AddRenderObject(quadBatch);
        }
        else
        {
            quadBatch = quadBatch;
        }
    }
    return quadBatch;
}

void CQuadBatchRenderGroup::Clear()
{
    for(auto object : m_objects)
    {
        BEATS_SAFE_DELETE(object);
    }
    CRenderGroup::Clear();
}
