#ifndef BEATS_COMPONENTS_PROPERTY_WXWIDGETSIMPLEMENT_COLORPROPERTYDESCRIPTION_H__INCLUDE
#define BEATS_COMPONENTS_PROPERTY_WXWIDGETSIMPLEMENT_COLORPROPERTYDESCRIPTION_H__INCLUDE

#include "wxwidgetsPropertyBase.h"

class CColorPropertyDescription : public CWxwidgetsPropertyBase
{
    typedef CWxwidgetsPropertyBase super;
public:
    CColorPropertyDescription(CSerializer* pSerializer);
    CColorPropertyDescription(const CColorPropertyDescription& rRef);

    virtual ~CColorPropertyDescription();

    virtual bool AnalyseUIParameterImpl(const std::vector<TString>& parameterUnit);
    virtual wxPGProperty* CreateWxProperty();
    virtual void SetValue(wxVariant& value, bool bSaveValue = true);
    virtual bool CopyValue(void* pSourceValue, void* pTargetValue) override;
    virtual bool IsDataSame(bool bWithDefaultOrXML);
    virtual CPropertyDescriptionBase* CreateNewInstance();
    virtual void GetValueAsChar(EValueType type, char* pOut) override;
    virtual bool GetValueByTChar(const TCHAR* pIn, void* pOutValue) override;
    virtual void Serialize(CSerializer& serializer, EValueType eValueType = eVT_SavedValue) override;
};

#endif