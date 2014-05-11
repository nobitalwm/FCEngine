#ifndef RENDER_RENDERSTATEPARAM_BOOLRENDERSTATEPARAM_H__INCLUDE
#define RENDER_RENDERSTATEPARAM_BOOLRENDERSTATEPARAM_H__INCLUDE

#include "RenderStateParamBase.h"


class CBoolRenderStateParam : public CRenderStateParamBase
{
    typedef CRenderStateParamBase super;
public:
    enum EBoolStateParam
    {
        eBSP_BlendEnable = 0x0BE2,//GL_BLEND
        eBSP_DepthEnable = 0x0B71,//GL_DEPTH_TEST
        eBSP_EdgeFlag,
        eBSP_LineSmooth = 0x0B20,//GL_LINE_SMOOTH
        eBSP_PointSmooth = 0x0B10,//GL_POINT_SMOOTH
        eBSP_PolySmooth = 0x0B41,//GL_POLYGON_SMOOTH
        eBSP_CullFace = 0xB44,//GL_CULL_FACE
        eBSP_StencilEnable = 0x0B90,//GL_STENCIL_TEST
        eBSP_ScissorEnable = 0x0C11,//GL_SCISSOR_TEST
        eBSP_VertexProgramTwoSideARB = 0x8643,//GL_VERTEX_PROGRAM_TWO_SIDE_ARB
        eBSP_WriteMaskDepth,
        eBSP_AlphaTest = 0x0BC0,//GL_ALPHA_TEST

        eBSP_Count = 11,
        eBSP_Force32Bit = 0xFFFFFFFF
    };

public:
    DECLARE_REFLECT_GUID(CBoolRenderStateParam, 0x47B45A18, CRenderStateParamBase)
public:
    CBoolRenderStateParam();
    virtual ~CBoolRenderStateParam();

    virtual void ReflectData(CSerializer& serializer) override;
    void SetType(EBoolStateParam type);
    EBoolStateParam GetType() const;
    void SetValue(bool bValue);
    bool GetValue() const;

    virtual void Apply() override;

    virtual ERenderState GetRenderStateType() const;

    virtual bool operator==( const CRenderStateParamBase& other ) const;

    virtual bool operator!=( const CRenderStateParamBase& other ) const;
private:
    EBoolStateParam m_type;
    bool m_bValue;
};

#endif