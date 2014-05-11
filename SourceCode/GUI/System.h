#ifndef GUI_SYSTEM_H__INCLUDE
#define GUI_SYSTEM_H__INCLUDE

#include "PublicDef.h"

//forward declaration
class CMouseEvent;
class CKeyboardEvent;
class CMaterial;
class CShaderProgram;
class CQuadBatchRenderGroup;
class CCamera;

namespace FCGUI
{
    //forward declaration
    class CWindow;

    class CSystem
    {
        BEATS_DECLARE_SINGLETON(CSystem);
    public:
        void Init();

        void Update(float deltaTime);
        void Render();

        bool InjectMouseEvent(CMouseEvent *event);
        bool InjectKeyboardEvent(CKeyboardEvent *event);

        void OnResolutionChanged(kmVec2 resolution);
        void OnResolutionChanged(kmScalar width, kmScalar height);
        kmVec2 GetResolution() const;

        SharePtr<CMaterial> CreateDefaultMaterial();
        CQuadBatchRenderGroup *GetRenderGroup();
        CCamera *GetCamera();

    private:
        void InitFontFace();
        void InitShader();
        void PreRender();
        void PostRender();

    private:
        kmVec2 m_resolution;
        CShaderProgram *m_pProgram;
        CQuadBatchRenderGroup *m_pRenderGroup;
        CCamera *m_pCamera;
    };
}

#endif