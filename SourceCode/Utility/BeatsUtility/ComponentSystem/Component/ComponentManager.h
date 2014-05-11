#ifndef BEATS_COMPONENTS_COMPONENT_COMPONENTMANAGER_H__INCLUDE
#define BEATS_COMPONENTS_COMPONENT_COMPONENTMANAGER_H__INCLUDE

class CComponentBase;
class CComponentEditorProxy;
class CIdManager;
class CPropertyDescriptionBase;
class TiXmlElement;
class CSerializer;
class CDependencyDescription;
class CComponentBase;
struct SDependencyResolver;
class CComponentProject;
class CComponentGraphic;
#include "ComponentManagerBase.h"

class CComponentManager : public CComponentManagerBase
{
    BEATS_DECLARE_SINGLETON(CComponentManager);
public:
    void SerializeTemplateData(CSerializer& serializer);
    size_t GetVersion();
    void Import(CSerializer& serializer);
    void ResetComponentContainer();
    virtual void ResolveDependency() override;

};

#endif