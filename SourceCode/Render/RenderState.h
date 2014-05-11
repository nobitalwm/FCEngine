#ifndef RENDER_RENDERSTATE_H__INCLUDE
#define RENDER_RENDERSTATE_H__INCLUDE

#include "RenderStateParam/RenderStateParamBase.h"
#include "RenderStateParam/BlendEquationRenderStateParam.h"
#include "RenderStateParam/BlendRenderStateParam.h"
#include "RenderStateParam/BoolRenderStateParam.h"
#include "RenderStateParam/ClockWiseRenderStateParam.h"
#include "RenderStateParam/CullModeRenderStateParam.h"
#include "RenderStateParam/FloatRenderStateParam.h"
#include "RenderStateParam/FunctionRenderStateParam.h"
#include "RenderStateParam/IntRenderStateParam.h"
#include "RenderStateParam/PolygonModeRenderStateParam.h"
#include "RenderStateParam/RectRenderStateParam.h"
#include "RenderStateParam/UintRenderStateParam.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"

class CRenderState : public CComponentBase
{
    typedef CComponentBase super;
    typedef std::vector< CRenderStateParamBase* > TParamStateVector;
    typedef TParamStateVector::iterator TParamStateVectorIter;
    typedef TParamStateVector::const_iterator TParamStateVectorIterConst;
    DECLARE_REFLECT_GUID(CRenderState, 0x71B04E1B, CComponentBase)

public:
    CRenderState();
    virtual ~CRenderState();

    virtual void ReflectData(CSerializer& serializer) override;
    void SetBoolState(GLenum state, bool bEnable);
    bool GetBoolState(GLenum state);

    void SetDepthMask(bool bWriteable);
    bool GetDepthMask() const;

    void SetEdgeFlag(bool bEdgeFlag);
    bool GetEdgetFlag() const;

    void SetActiveTexture(GLenum activeTexture);
    GLenum GetActiveTexture() const;

    void SetBlendFuncSrcFactor(GLenum src);
    void SetBlendFuncTargetFactor(GLenum target);
    GLenum GetBlendSrcFactor();
    GLenum GetBlendTargetFactor();
    void SetBlendEquation(GLenum func);
    GLenum GetBlendEquation();
    void SetAlphaFunc(GLenum func);
    GLenum GetAlphaFunc();
    void SetAlphaRef(float fRef);
    float GetAlphaRef();

    void SetShaderProgram(GLuint program);
    GLuint GetShaderProgram()const;

    void SetFrontFace(GLenum frontFace);
    GLenum GetFrontFace();

    void SetCullFace(GLenum cullFace);
    GLenum GetCullFace();

    void SetDepthNear(float fDepthNear);
    void SetDepthFar(float fDepthFar);
    float GetDepthNear();
    float GetDepthFar();
    void SetDepthFunc(GLenum func);
    GLenum GetDepthFunc();

    void SetStencilFunc(GLenum func);
    GLenum GetStencilFunc();
    void SetStencilReference(GLint nRef);
    GLint GetStencilReference();
    void SetStencilValueMask(GLint nValueMask);
    GLint GetStencilValueMask();
    void SetClearStencil(GLint nClearValue);
    GLint GetClearStencil();
    void SetStencilOp( GLenum fail, GLenum zFail, GLenum zPass );
    void GetStencilOp( GLenum& fail, GLenum& zFail, GLenum& zPass );
    
    void SetScissorRect(kmScalar x, kmScalar y, kmScalar width, kmScalar height);
    void GetScissorRect(kmVec4 &rect) const;

    void SetShadeModel(GLenum shadeModel);
    GLenum GetShadeModel();

    void SetPolygonMode( GLenum face, GLenum mode );
    void GetPolygonMode( GLenum& face, GLenum& mode );

    void SetColor( CUintRenderStateParam::EUintStateParam type, GLclampf r, GLclampf g, GLclampf b, GLclampf a );
    void GetColor( CUintRenderStateParam::EUintStateParam type , GLclampf& r, GLclampf& g, GLclampf& b, GLclampf& a );



    void Restore();

    bool operator==( const CRenderState& other ) const;

    bool operator!=( const CRenderState& other ) const;

private:

    CRenderStateParamBase* GetRenderStateParamBasePtr( ERenderState state ) const;

    CFloatRenderStateParam* GetFloatRenderStateParamPtr( CFloatRenderStateParam::EFloatStateParam type ) const;

    CFunctionRenderStateParam* GetFuncRenderStateParamPtr( CFunctionRenderStateParam::EFunctionStateParam type ) const;

    CIntRenderStateParam* GetIntRenderStateParamPtr( CIntRenderStateParam::EIntStateParam type ) const;

    CBoolRenderStateParam* GetBoolRenderStateParamPtr( CBoolRenderStateParam::EBoolStateParam type ) const;

    CUintRenderStateParam* GetUnitRenderStateParamPtr( CUintRenderStateParam::EUintStateParam type ) const;
    
    CRenderStateParamBase* GetRectRenderStateParamPtr( CRectRenderStateParam::ERectStateParam type) const;

private:
    bool m_bDepthMark;
    bool m_bEdgeFlag;
    size_t m_uCurrShaderProgram;
    size_t m_uCurrActiveTexture;
    TParamStateVector m_pRenderStateParams;
    TParamStateVector m_boolRenderStateVector;
    TParamStateVector m_funcRenderStateVector;
    TParamStateVector m_floatRenderStateVector;
    TParamStateVector m_intRenderStateVector;
    TParamStateVector m_unitRenderStateVector;
    TParamStateVector m_rectRenderStateVector;
};

#endif