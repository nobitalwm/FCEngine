#include "stdafx.h"
#include "RenderGroup.h"
#include "RenderObject.h"

CRenderGroup::CRenderGroup(int ID)
    : m_nID(ID)
{

}

CRenderGroup::~CRenderGroup()
{

}

void CRenderGroup::PreRender()
{

}

void CRenderGroup::Render()
{
    for(auto object : m_objects)
    {
        object->PreRender();
        object->Render();
        object->PostRender();
    }
}

void CRenderGroup::PostRender()
{

}

int CRenderGroup::ID() const
{
    return m_nID;
}

void CRenderGroup::AddRenderObject(CRenderObject *object)
{
    m_objects.push_back(object);
}

void CRenderGroup::Clear()
{
    m_objects.clear();
}
