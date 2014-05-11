#ifndef RENDER_QUAD_BATCH_RENDER_GROUP_H__INCLUDE
#define RENDER_QUAD_BATCH_RENDER_GROUP_H__INCLUDE

#include "RenderGroup.h"

class CQuadBatch;
class CMaterial;

class CQuadBatchRenderGroup : public CRenderGroup
{
public:
    CQuadBatchRenderGroup(int ID);
    virtual ~CQuadBatchRenderGroup();

    void AddQuad(const CQuadP &quadP, const CQuadT &quadT, 
        const kmMat4 &transform, SharePtr<CMaterial> material);

    void AddQuad(const CQuadPTC &quad, SharePtr<CMaterial> material);

    virtual void AddRenderObject( CRenderObject *object );

    virtual void Clear();

private:
    CQuadBatch *GetQuadBatch(SharePtr<CMaterial> material);

};

#endif // !RENDER_QUAD_BATCH_RENDER_GROUP_H__INCLUDE
