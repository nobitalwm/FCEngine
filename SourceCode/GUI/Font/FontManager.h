#ifndef GUI_FONT_FONT_MANAGER_H__INCLUDE
#define GUI_FONT_FONT_MANAGER_H__INCLUDE

class CShaderProgram;
class CMaterial;
class CQuadBatchRenderGroup;

namespace FCGUI
{
    //forward declaration
    class CFontFace;
    class CFontGlyph;

    class CFontManager
    {
        BEATS_DECLARE_SINGLETON(CFontManager);
    public:
        CFontFace *CreateFace(const TString &name, const TString &file, int sizeInPt, int dpi = -1);
        CFontFace *GetFace(const TString &name);
        void RenderText(const TString &text, kmScalar x, kmScalar y, 
            const TString &faceName, CColor color = 0x000000FF);
        SharePtr<CMaterial> CreateDefaultMaterial();
        CQuadBatchRenderGroup *GetRenderGroup();

    private:
        std::map<TString, CFontFace *> m_faceMap;
        CShaderProgram *m_pProgram;
        CQuadBatchRenderGroup *m_pRenderGroup;
    };
}

#endif