#ifndef GUI_FONT_FONT_FACE_H__INCLUDE
#define GUI_FONT_FONT_FACE_H__INCLUDE

class CTexture;
class CMaterial;

namespace FCGUI
{
    class CFont;

    class CFontGlyph
    {
    public:
        kmScalar u, v;
        kmScalar width, height;
        SharePtr<CTexture> texture;
    };

    class CFontFace
    {
    public:
        CFontFace(const TString &name, const TString &file, int size, int dpi = -1);
        ~CFontFace();

        void PrepareCharacters(const TString &chars);
        void RenderText(const TString &text, kmScalar x, kmScalar y, 
            CColor color = 0x000000FF, const kmMat4 *transform = nullptr);
        std::vector<CFontGlyph *> GetGlyphs(const TString &text);
        CFontGlyph *GetGlyph(unsigned long character) const;
        const TString& GetName()const;   

    private:
        void NewPage();
        void SetSize();
        void DrawGlyph(CFontGlyph *glyph, kmScalar x, kmScalar y, 
            const CQuadC &colorRect, const kmMat4 *transform = nullptr) const;

    private:
        TString m_strName;
        SharePtr<CFont> m_pFont;
        std::map<unsigned long, CFontGlyph *> m_glyphMap;
        std::vector<SharePtr<CTexture>> m_textures;
        std::map<GLuint, SharePtr<CMaterial>> m_materialMap;
        int m_nCurrPage;
        size_t m_uCurrX;
        size_t m_uCurrY;
        int m_nSize;
        int m_nDpi;
        int m_nLineHeight;    //maxheight of current font face
        int m_nAscender;  //distance from top to baseline

        static const int PAGE_WIDTH = 1024;
        static const int PAGE_HEIGHT = 1024;
    };
}

#endif