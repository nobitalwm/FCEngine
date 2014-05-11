#ifndef RENDER_TEXTURE_FRAG_MANAGER_H__INCLUDE
#define RENDER_TEXTURE_FRAG_MANAGER_H__INCLUDE

class CTextureFrag;
class CTexture;
class CTextureAtlas;

class CTextureFragManager
{
    BEATS_DECLARE_SINGLETON(CTextureFragManager);
public:
    CTextureFrag *GetTextureFrag(const TString &atlasName, const TString &fragName);
    CTextureAtlas *CreateTextureAtlas(const TString &textureFileName);
    void AddTextureAtlas(CTextureAtlas *atlas);
    void RemoveTextureAtlas(const TString &atlasName);

private:
    std::map<TString, CTextureAtlas *> m_textureAtlases;
    std::set<CTextureAtlas *> m_managedAtlases;
};

#endif