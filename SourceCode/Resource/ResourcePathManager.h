#ifndef FCENGINEEDITOR_RESOURCEPATHMANAGER_H__INCLUDE
#define FCENGINEEDITOR_RESOURCEPATHMANAGER_H__INCLUDE

 
static const TCHAR* pszCocosResourcePathName[] =
{
    _T("../SourceCode"),
    _T("../Resource"),
    _T("../SourceCode/Shader"),
    _T("../Resource/Animation"),
    _T("../Resource/SpriteAnimation"),
    _T("../Resource/Skin"),
    _T("../Resource/Skeleton"),
    _T("../Resource/Material"),
    _T("../Resource/Texture"),
    _T("../Resource/Particle"),
    _T("../Resource/Font"),
    _T("../Resource/Language"),

    _T(""),
};

class CResourcePathManager
{
    BEATS_DECLARE_SINGLETON(CResourcePathManager);
public:
    enum EResourcePathType
    {
        eRPT_SourceCode,
        eRPT_Resource,
        eRPT_Shader,
        eRPT_Animation,
        eRPT_SpriteAnimation,
        eRPT_Skin,
        eRPT_Skeleton,
        eRPT_Material,
        eRPT_Texture,
        eRPT_Particle,
        eRPT_Font,
        eRPT_Language,

        eRPT_Work,// Keep work directory at the end for cocos cross platform.
        eRPT_Count,
    };

    void Init();
    const TString GetResourcePath(CResourcePathManager::EResourcePathType type) const;
    CResourcePathManager::EResourcePathType GetResourcePathType(EResourceType type) const;
    TString GetFullPathForFilename(const std::string &filename);
};
#endif