#include "stdafx.h"
#include "RenderGroupManager.h"
#include "RenderGroup.h"
#include "QuadBatchRenderGroup.h"

CRenderGroupManager *CRenderGroupManager::m_pInstance = nullptr;

CRenderGroupManager::CRenderGroupManager()
{
}

CRenderGroupManager::~CRenderGroupManager()
{
    for(auto group : m_groupMap)
    {
        BEATS_SAFE_DELETE(group.second);
    }
}

CRenderGroup *CRenderGroupManager::GetRenderGroup(int groupID)
{
    CRenderGroup *group = nullptr;
    auto itr = m_groupMap.find(groupID);
    if(itr == m_groupMap.end())
    {
        group = createRenderGroup(groupID);
    }
    else
    {
        group = itr->second;
    }
    return group;
}

void CRenderGroupManager::Render()
{
    for(auto group : m_groupMap)
    {
        group.second->PreRender();
        group.second->Render();
        group.second->PostRender();
    }
}

void CRenderGroupManager::Clear()
{
    for(auto group : m_groupMap)
    {
        group.second->Clear();
    }
}

CRenderGroup *CRenderGroupManager::createRenderGroup(int groupID)
{
    CRenderGroup *group = nullptr;
    if( groupID == LAYER_GUI ||
        groupID == LAYER_2D)
    {
        group = new CQuadBatchRenderGroup(groupID);
    }
    else
    {
        group = new CRenderGroup(groupID);
    }
    m_groupMap[groupID] = group;
    return group;
}

