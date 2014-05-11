#ifndef RENDER_RENDERSTATEPARAM_RECTRENDERSTATEPARAM_H__INCLUDE
#define RENDER_RENDERSTATEPARAM_RECTRENDERSTATEPARAM_H__INCLUDE

#include "RenderStateParamBase.h"


class CRectRenderStateParam : public CRenderStateParamBase
{
    typedef CRenderStateParamBase super;
public:
    enum ERectStateParam
    {
        eRSP_VIEWPORT,
        eRSP_SCISSOR,

        eBSP_Count,
        eBSP_Force32Bit = 0xFFFFFFFF
    };

public:
    DECLARE_REFLECT_GUID(CRectRenderStateParam, 0xC562DFE1, CRenderStateParamBase)
public:
    CRectRenderStateParam();
    virtual ~CRectRenderStateParam();
    virtual void ReflectData(CSerializer& serializer) override;
    void SetType(ERectStateParam type);
    ERectStateParam GetType() const;
    void SetValue(kmScalar x, kmScalar y, kmScalar width, kmScalar height);
    kmVec4 GetValue() const;

    virtual void Apply() override;

    virtual ERenderState GetRenderStateType() const override;

    virtual bool operator ==( const CRenderStateParamBase& other ) const override;

    virtual bool operator !=( const CRenderStateParamBase& other ) const override;

private:
    ERectStateParam m_type;
    kmVec4 m_value;
};

#endif