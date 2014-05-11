#include "stdafx.h"
#include "BaseRenderer.h"
#include "GUI/Window/Window.h"
#include "Render/TextureFrag.h"
#include "Render/Texture.h"
#include "Render/QuadBatchRenderGroup.h"
#include "Render/TextureFragManager.h"
#include "Render/Material.h"
#include "Render/RenderState.h"
#include "GUI/Event/WindowEvent.h"
#include "GUI/System.h"

using namespace FCGUI;

CBaseRenderer::CBaseRenderer()
    : m_pWindow(nullptr)
{
}

CBaseRenderer::~CBaseRenderer()
{
    for(auto layers : m_stateLayers)
    {
        for(auto layer : layers.second)
        {
            BEATS_SAFE_DELETE(layer);
        }
    }
}

void CBaseRenderer::SetWindow(CWindow *window)
{
    m_pWindow = window;

    auto windowEventHandler = std::bind(&CBaseRenderer::OnWindowEvent, this, std::placeholders::_1); 
    m_pWindow->SubscribeEvent(CWindow::eET_EVENT_SIZED, windowEventHandler);
    m_pWindow->SubscribeEvent(CWindow::eET_EVENT_MOVED, windowEventHandler);
    m_pWindow->SubscribeEvent(CWindow::eET_EVENT_ROTATED, windowEventHandler);
    m_pWindow->SubscribeEvent(CWindow::eET_EVENT_SCALED, windowEventHandler);
    m_pWindow->SubscribeEvent(CWindow::eET_EVENT_ANCHOR_CHANGED, windowEventHandler);

    SetVertices(m_pWindow);
}

void CBaseRenderer::AddLayer(CTextureFrag *layer, unsigned int state)
{
    m_stateLayers[state].push_back(new CRenderLayer(layer));
}

void CBaseRenderer::AddLayer(const TString &textureFragName, unsigned int state)
{
    size_t splitterPos = textureFragName.find(_T('/'));
    TString atlasName = textureFragName.substr(0, splitterPos);
    TString fragName = textureFragName.substr(splitterPos + 1);
    CTextureFrag *frag = CTextureFragManager::GetInstance()->GetTextureFrag(atlasName, fragName);
    BEATS_ASSERT(frag);
    AddLayer(frag, state);
}

void CBaseRenderer::AddLayer(const CRenderLayer::TFrameList &frames, unsigned int state)
{
    m_stateLayers[state].push_back(new CRenderLayer(frames));
}

CRenderLayer *CBaseRenderer::GetLayer(size_t index, unsigned int state)
{
    CRenderLayer *layer = nullptr;
    auto itr = m_stateLayers.find(state);
    if(itr != m_stateLayers.end())
    {
        if(index < itr->second.size())
        {
            layer = itr->second[index];
        }
    }
    return layer;
}

void CBaseRenderer::Render(const kmMat4 &parentTransform) const
{
    if(!m_pWindow || !m_pWindow->IsVisible()) return;

    kmMat4 worldTransform;
    kmMat4Multiply(&worldTransform, &parentTransform, &m_pWindow->Transform());

    RenderLayers(worldTransform);
    RenderChildren(worldTransform);
}

void CBaseRenderer::RenderLayers( const kmMat4 &parentTransform ) const
{
    const TLayerList *layers = nullptr;
    auto itr = m_stateLayers.find(m_pWindow->CurrState());
    if(itr != m_stateLayers.end() && !itr->second.empty())
    {
        layers = &itr->second;
    }
    else
    {
        auto itrNormal = m_stateLayers.find(CWindow::eST_STATE_NORMAL);
        if(itrNormal != m_stateLayers.end())
        {
            layers = &itrNormal->second;
        }
    }
    if(layers)
    {
        for(auto layer : *layers)
        {
            if(m_pWindow->IsClipping())
            {
                layer->GetMaterial()->GetRenderState()->SetBoolState(GL_SCISSOR_TEST, true);
                kmVec2 pos;
                kmVec2 size;
                m_pWindow->GetScissorRect(pos, size);
                layer->GetMaterial()->GetRenderState()->SetScissorRect(pos.x, pos.y, size.x, size.y);
            }
            else
            {
                layer->GetMaterial()->GetRenderState()->SetBoolState(GL_SCISSOR_TEST, false);
            }
            CSystem::GetInstance()->GetRenderGroup()->AddQuad(m_quad, 
                layer->GetTextureFrag()->Quad(), parentTransform, layer->GetMaterial());
        }
    }
}

void CBaseRenderer::RenderChildren(const kmMat4 &parentTransform) const
{
    BEATS_ASSERT(m_pWindow);

    m_pWindow->Traverse([&parentTransform](CWindow *window){
        CBaseRenderer *renderer = window->Renderer();
        if(renderer)
        {
            renderer->Render(parentTransform);
        }
        return false;
    });    
}

void CBaseRenderer::OnWindowEvent(CBaseEvent *event)
{
    WindowEvent *eventWindow = static_cast<WindowEvent *>(event);
    switch(event->Type())
    {
    case CWindow::eET_EVENT_SIZED:
        SetVertices(eventWindow->SourceWindow());
        break;
    case CWindow::eET_EVENT_ANCHOR_CHANGED:
        SetVertices(eventWindow->SourceWindow());
        break;
    }
}

void CBaseRenderer::SetVertices(const CWindow *window)
{
    BEATS_ASSERT(window);

    kmScalar anchorX = window->RealSize().x * window->Anchor().x;
    kmScalar anchorY = window->RealSize().y * window->Anchor().y;

    //set vertex
    m_quad.tl.x = 0 - anchorX;
    m_quad.tl.y = 0 - anchorY;
    m_quad.tr.x = window->RealSize().x - anchorX;
    m_quad.tr.y = m_quad.tl.y;
    m_quad.bl.x = m_quad.tl.x;
    m_quad.bl.y = window->RealSize().y - anchorY;
    m_quad.br.x = m_quad.tr.x;
    m_quad.br.y = m_quad.bl.y;
}
