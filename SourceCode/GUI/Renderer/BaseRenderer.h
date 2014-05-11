#ifndef GUI_RENDERER_BASE_RENDERER_H__INCLUDE
#define GUI_RENDERER_BASE_RENDERER_H__INCLUDE 

#include "RendererDefs.h" 
#include "Render/CommonTypes.h"
#include "RenderLayer.h"

//forward declaration
class CTextureFrag;
class CBaseEvent;

namespace FCGUI
{
    //forward declaration
    class CWindow;
    class CRenderLayer;

    //rendering children and layers
    class CBaseRenderer
    {
        DEFINE_RENDERER_TYPE(RENDERER_BASE);
    public:
        CBaseRenderer();
        virtual ~CBaseRenderer();

        void SetWindow(CWindow *window);

        virtual void Render(const kmMat4 &parentTransform) const;

        void AddLayer(const TString &textureFragName, unsigned int state = 0);
        void AddLayer(CTextureFrag *textureFrag, unsigned int state = 0);
        void AddLayer(const CRenderLayer::TFrameList &frames, unsigned int state = 0);

        CRenderLayer *GetLayer(size_t index, unsigned int state = 0);

    protected:
        virtual void SetVertices(const CWindow *window);

        virtual void RenderChildren(const kmMat4 &parentTransform) const;
        virtual void RenderLayers(const kmMat4 &worldTransform) const;

    private:
        void OnWindowEvent(CBaseEvent *event);

    protected:
        typedef std::vector<CRenderLayer *> TLayerList;
        typedef std::map<unsigned int, TLayerList> TStateLayerList;

        CWindow *m_pWindow;

        CQuadP m_quad;
        TStateLayerList m_stateLayers;
    };
}

#endif