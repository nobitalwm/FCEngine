#ifndef RENDER_QUAD_BATCH_H__INCLUDE
#define RENDER_QUAD_BATCH_H__INCLUDE

#include "RenderObject.h"
#include "CommonTypes.h"

class CMaterial;
class CQuadBatch : public CRenderObject
{
public:
    CQuadBatch(SharePtr<CMaterial> material);
    virtual ~CQuadBatch();

    virtual void PreRender() override;

    virtual void Render() override;

    virtual void PostRender() override;

    void AddQuad(const CQuadP &quadP, const CQuadT &quadT, const kmMat4 &transform);

    void AddQuad(const CQuadPTC &quad);

    SharePtr<CMaterial> GetMaterial() const;

    void Clear();

private:
    void setupVAO();
    void updateVBO();

private:
    static const size_t MAX_QUADS = 1024;
    SharePtr<CMaterial> m_pMaterial;
    CQuadPTC m_quads[MAX_QUADS];
    size_t m_uCount;
    GLuint m_VAO, m_VBO[2];
};

#endif