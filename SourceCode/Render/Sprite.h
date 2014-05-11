#ifndef RENDER_SPRITE_H__INCLUDE
#define RENDER_SPRITE_H__INCLUDE

#include "RenderObject.h"
#include "CommonTypes.h"
#include "Geometry.h"

class CTexture;
class CTextureFrag;
class CMaterial;

class CSprite : public CRenderObject
{
public:
    CSprite(CTextureFrag *textureFrag);
    CSprite(const TString &textureAtlasName, const TString &textureFragName);
    CSprite(const TString &textureAtlasName, const TString &textureFragName, const kmVec2 &size);
    CSprite(const TString &textureAtlasName, const TString &textureFragName, 
        const kmVec2 &size, const kmVec2 &origin);
    virtual ~CSprite();

    virtual void PreRender() override;

    virtual void Render() override;

    virtual void PostRender() override;

    void SetQuad(const CQuadP &quad);
    void SetOriginSize(const kmVec2 &origin, const kmVec2 &size);

    const CQuadP &QuadP() const;
    const CQuadT &QuadT() const;
    SharePtr<CTexture> Texture() const;
    SharePtr<CMaterial> Material();

protected:
    CQuadP m_quad;
    CTextureFrag *m_textureFrag;
    SharePtr<CMaterial> m_material;
};

#endif