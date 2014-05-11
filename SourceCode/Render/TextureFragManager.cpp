#include "stdafx.h"
#include "TextureFragManager.h"
#include "TextureFrag.h"
#include "Texture.h"
#include "TextureAtlas.h"

CTextureFragManager *CTextureFragManager::m_pInstance = nullptr;

CTextureFragManager::CTextureFragManager()
{

}

CTextureFragManager::~CTextureFragManager()
{
    for(auto atlas : m_managedAtlases)
    {
        atlas->Unload();
        BEATS_SAFE_DELETE(atlas);
    }
}

CTextureFrag *CTextureFragManager::GetTextureFrag(const TString &atlasName, const TString &fragName)
{
    auto itr = m_textureAtlases.find(atlasName);
    return itr != m_textureAtlases.end() ? itr->second->GetTextureFrag(fragName) : nullptr;
}

CTextureAtlas *CTextureFragManager::CreateTextureAtlas(const TString &textureFileName)
{
    CTextureAtlas *atlas = new CTextureAtlas;
    atlas->SetFilePath(textureFileName);
    atlas->Load();
    m_managedAtlases.insert(atlas);
    return atlas;
}

void CTextureFragManager::AddTextureAtlas(CTextureAtlas *atlas)
{
    BEATS_ASSERT(m_textureAtlases.find(atlas->Name()) == m_textureAtlases.end());
    m_textureAtlases[atlas->Name()] = atlas;
}

void CTextureFragManager::RemoveTextureAtlas(const TString &atlasName)
{
    m_textureAtlases.erase(atlasName);
}
