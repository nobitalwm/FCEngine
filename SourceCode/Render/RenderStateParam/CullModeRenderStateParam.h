#ifndef RENDER_RENDERSTATEPARAM_CULLMODERENDERSTATEPARAM_H__INCLUDE
#define RENDER_RENDERSTATEPARAM_CULLMODERENDERSTATEPARAM_H__INCLUDE
#include "RenderStateParamBase.h"

class CCullModeRenderStateParam : public CRenderStateParamBase
{
    typedef CRenderStateParamBase super;
public:
    enum ECullModeType
    {
        eCMT_NONE = 0, //GL_NONE
        eCMT_FRONT = 0x0404, //GL_FRONT
        eCMT_BACK = 0x0405, //GL_BACK
        eCMT_FRONT_AND_BACK = 0x0408, //GL_FRONT_AND_BACK

        eCMT_Count = 4, 
        eCMT_Force32Bit = 0xFFFFFFFF
    };

    DECLARE_REFLECT_GUID(CCullModeRenderStateParam, 0x1A0373B6, CRenderStateParamBase)
public:
    CCullModeRenderStateParam();
    virtual ~CCullModeRenderStateParam();

    virtual void ReflectData(CSerializer& serializer) override;
    virtual void Apply() override;

    virtual ERenderState GetRenderStateType() const;

    void SetCullModeType( ECullModeType type );
    ECullModeType GetCullModeType() const;

    virtual bool operator==( const CRenderStateParamBase& other ) const;

    virtual bool operator!=( const CRenderStateParamBase& other ) const;
private:
    ECullModeType m_nValue;
};

#endif