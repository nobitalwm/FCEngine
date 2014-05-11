#ifndef FCENGINEEDITOR_COMPONENTGRAPHICS_GL_H__INCLUDE
#define FCENGINEEDITOR_COMPONENTGRAPHICS_GL_H__INCLUDE

#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentGraphic.h"

namespace FCGUI
{
    class CFontFace;
}
class CMaterial;

class CComponentGraphic_GL : public CComponentGraphic
{
    typedef CComponentGraphic super;
public:
    CComponentGraphic_GL();
    virtual ~CComponentGraphic_GL();

    void CreateMaterials();
    SharePtr<CMaterial> CreateMaterial(const TString &textureFileName);
    virtual void GetDependencyPosition(size_t uDependencyIndex, int* pOutX, int* pOutY);
    virtual EComponentAeraRectType HitTestForAreaType( int x, int y, void** pReturnData);
    virtual CComponentGraphic* Clone();

private:
    virtual void DrawHead(float cellSize);
    virtual void DrawDependencies(float cellSize);
    virtual void DrawDependencyLine(float cellSize, const CDependencyDescription* pDependency);
    virtual void DrawSelectedRect(float cellSize);

private:
    FCGUI::CFontFace*   m_pFont;
    SharePtr<CMaterial> m_pMaterials[eCT_Count];
};

#endif