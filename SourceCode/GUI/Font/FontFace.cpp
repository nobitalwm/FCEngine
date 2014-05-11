#include "stdafx.h"
#include "FontFace.h"
#include "Font.h"
#include "Render/Texture.h"
#include "Render/QuadBatchRenderGroup.h"
#include "Render/Material.h"
#include "Resource/ResourceManager.h"
#include "FontManager.h"

#define FT_SHIFT_NUM 6;

//next multiple of 8
inline int nextMOE(int x)
{
    return (x + 7) & 0xFFFFFFF8;
}

using namespace FCGUI;

CFontFace::CFontFace(const TString &name, const TString &file, int size, int dpi/*= -1*/)
    : m_strName(name)
    , m_nCurrPage(-1)
    , m_uCurrX(0)
    , m_uCurrY(0)
    , m_nLineHeight(0)
    , m_nAscender(0)
    , m_nSize(size)
    , m_nDpi(dpi)
    , m_pFont(CResourceManager::GetInstance()->GetResource<CFont>(file, false))
{
    NewPage();
}

CFontFace::~CFontFace()
{
    for(auto glyph : m_glyphMap)
    {
        BEATS_SAFE_DELETE(glyph.second);
    }
}

void CFontFace::NewPage()
{
    SharePtr<CTexture> texture = new CTexture;
    texture->InitWithData(nullptr, 0, PixelFormat::A8, PAGE_WIDTH, PAGE_HEIGHT);
    ++m_nCurrPage;
    m_textures.push_back(texture);

    SharePtr<CMaterial> material = CFontManager::GetInstance()->CreateDefaultMaterial();
    material->SetTexture(0, texture);
    m_materialMap[texture->ID()] = material;
}

void CFontFace::SetSize()
{
    FT_Error err = 0;
    if (m_nDpi == -1)
    {
        err = FT_Set_Pixel_Sizes(m_pFont->Face(), m_nSize, m_nSize);
    }
    else
    {
        FT_F26Dot6 sizeTrans = m_nSize << FT_SHIFT_NUM;
        err = FT_Set_Char_Size(m_pFont->Face(), sizeTrans, sizeTrans, m_nDpi, m_nDpi);
    }

    m_nAscender = m_pFont->Face()->size->metrics.ascender >> FT_SHIFT_NUM;
    long descender = m_pFont->Face()->size->metrics.descender >> FT_SHIFT_NUM;
    m_nLineHeight = m_nAscender - descender;

    BEATS_ASSERT(!err);
}

void CFontFace::PrepareCharacters( const TString &chars )
{
    SetSize();

#ifndef _UNICODE
    //TODO: translate chars to unicode
#pragma error("non-unicode unsupported")
#endif

    FT_Error err = 0;

    for(size_t i = 0; i < chars.size(); ++i)
    {
        wchar_t character = chars[i];

        if(m_glyphMap.find(character) != m_glyphMap.end())
            continue;

        err = FT_Load_Char(m_pFont->Face(), character, FT_LOAD_RENDER);
        BEATS_ASSERT(!err);

        FT_Bitmap &bitmap = m_pFont->Face()->glyph->bitmap;
        FT_Glyph_Metrics &metrics = m_pFont->Face()->glyph->metrics;
        long x = metrics.horiBearingX >> FT_SHIFT_NUM;
        long y = metrics.horiBearingY >> FT_SHIFT_NUM;
        long xAdvance = metrics.horiAdvance >> FT_SHIFT_NUM;
        long width = metrics.width >> FT_SHIFT_NUM;
        long height = metrics.height >> FT_SHIFT_NUM;
        width, height;

        if(x < 0)
            x =  0;
        if(xAdvance < x + width)
            xAdvance = x + width;

        long xOffset = x;
        long yOffset = m_nAscender - y;

        long destWidth = nextMOE(bitmap.width);
        long destHeight = nextMOE(bitmap.rows);

        if(m_uCurrX + xOffset + destWidth > PAGE_WIDTH)
        {
            m_uCurrX = 0;
            m_uCurrY += m_nLineHeight;
            if(m_uCurrY + yOffset + destHeight > PAGE_HEIGHT)
            {
                m_uCurrY = 0;
                NewPage();
            }
        }

        unsigned char *data = bitmap.buffer;
        bool needRealloc = bitmap.width != bitmap.pitch
            || destWidth != bitmap.width || destHeight != bitmap.rows;
        if(needRealloc)
        {
            data = new unsigned char[destWidth * destHeight];
            for(long i = 0; i < destHeight; ++i)
            {
                for(long j = 0; j < destWidth; ++j)
                {
                    data[i * destWidth + j] = (i < bitmap.rows && j < bitmap.width) ?
                        bitmap.buffer[i * bitmap.pitch + j] : 0;
                }
            }
        }

        SharePtr<CTexture> texture = m_textures[m_nCurrPage];
        texture->UpdateSubImage(m_uCurrX + xOffset, m_uCurrY + yOffset, destWidth, destHeight, data);

        if(needRealloc)
        {
            BEATS_SAFE_DELETE_ARRAY(data);
        }

        CFontGlyph *glyph = new CFontGlyph;
        glyph->width = (float)xAdvance;
        glyph->height = (float)m_nLineHeight;
        glyph->u = (float)m_uCurrX / PAGE_WIDTH;
        glyph->v = (float)m_uCurrY / PAGE_HEIGHT;
        glyph->texture = m_textures[m_nCurrPage];
        m_glyphMap[character] = glyph;

        m_uCurrX += xAdvance;
    }
}

void CFontFace::RenderText(const TString &text, kmScalar x, kmScalar y, 
                          CColor color, const kmMat4 *transform)
{
    CQuadC quadC;
    quadC.tl = color;
    quadC.tr = color;
    quadC.bl = color;
    quadC.br = color;

    auto glyphs = GetGlyphs(text);
    for(auto glyph : glyphs)
    {
        DrawGlyph(glyph, x, y, quadC, transform);
        x += glyph->width;
    }
}

void CFontFace::DrawGlyph(CFontGlyph *glyph, kmScalar x, kmScalar y, 
                         const CQuadC &colorRect, const kmMat4 *transform) const
{
    CQuadPTC quad;
    quad.tl.position.x = x;
    quad.tl.position.y = y;
    quad.tr.position.x = x + glyph->width;
    quad.tr.position.y = quad.tl.position.y;
    quad.bl.position.x = quad.tl.position.x;
    quad.bl.position.y = y + glyph->height;
    quad.br.position.x = quad.tr.position.x;
    quad.br.position.y = quad.bl.position.y;
    if(transform)
    {
        kmVec3Transform(&quad.tl.position, &quad.tl.position, transform);
        kmVec3Transform(&quad.tr.position, &quad.tr.position, transform);
        kmVec3Transform(&quad.bl.position, &quad.bl.position, transform);
        kmVec3Transform(&quad.br.position, &quad.br.position, transform);
    }
    quad.tl.tex.u = glyph->u;
    quad.tl.tex.v = glyph->v;
    quad.tr.tex.u = glyph->u + glyph->width / glyph->texture->Width();
    quad.tr.tex.v = quad.tl.tex.v;
    quad.bl.tex.u = quad.tl.tex.u;
    quad.bl.tex.v = glyph->v + glyph->height / glyph->texture->Height();
    quad.br.tex.u = quad.tr.tex.u;
    quad.br.tex.v = quad.bl.tex.v;
    quad.tl.color = colorRect.tl;
    quad.tr.color = colorRect.tr;
    quad.bl.color = colorRect.bl;
    quad.br.color = colorRect.br;

    auto itr = m_materialMap.find(glyph->texture->ID());
    BEATS_ASSERT(itr != m_materialMap.end());

    CFontManager::GetInstance()->GetRenderGroup()->AddQuad(quad, itr->second);
}

CFontGlyph *CFontFace::GetGlyph( unsigned long character ) const
{
    auto itr = m_glyphMap.find(character);
    return itr != m_glyphMap.end() ? itr->second : nullptr;
}

std::vector<CFontGlyph *> CFontFace::GetGlyphs(const TString &text)
{
    PrepareCharacters(text);
    std::vector<CFontGlyph *> glyphs;
    for(auto character : text)
    {
        CFontGlyph *glyph = GetGlyph(character);
        if(glyph)
            glyphs.push_back(glyph);
    }
    return glyphs;
}

const TString& CFontFace::GetName()const
{
    return m_strName;
}
