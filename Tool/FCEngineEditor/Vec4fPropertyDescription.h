#ifndef BEATS_COMPONENTS_PROPERTY_WXWIDGETSIMPLEMENT_KMDATAPROPERTY_VEC4FPROPERTYDESCRIPTION_H__INCLUDE
#define BEATS_COMPONENTS_PROPERTY_WXWIDGETSIMPLEMENT_KMDATAPROPERTY_VEC4FPROPERTYDESCRIPTION_H__INCLUDE

#include "ListPropertyDescription.h"

class CVec4PropertyDescription : public CListPropertyDescription
{
    typedef CListPropertyDescription super;
public:
    CVec4PropertyDescription(CSerializer* pSerializer);
    CVec4PropertyDescription(const CVec4PropertyDescription& rRef);

    virtual ~CVec4PropertyDescription();

    virtual bool AnalyseUIParameterImpl(const std::vector<TString>& parameterUnit) override;
    virtual bool IsDataSame(bool bWithDefaultOrXML) override;
    virtual CPropertyDescriptionBase* CreateNewInstance() override;
    virtual CPropertyDescriptionBase* Clone(bool bCloneValue) override;
    virtual bool IsContainerProperty() override;
    virtual void LoadFromXML( TiXmlElement* pNode ) override;
    virtual void Serialize(CSerializer& serializer, EValueType eValueType = eVT_SavedValue) override;

private:
    virtual void ResetChildName() override;
    virtual void GetCurrentName(wxString& strName) override;

public:
    static SharePtr<SBasicPropertyInfo> m_pWBasicPropertyInfo;
};

#endif