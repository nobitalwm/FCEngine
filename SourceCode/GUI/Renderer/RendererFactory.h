#ifndef GUI_RENDERER_RENDERER_FACTORY_H__INCLUDE
#define GUI_RENDERER_RENDERER_FACTORY_H__INCLUDE

namespace FCGUI
{
    class CBaseRenderer;

    class RendererFactory
    {
    public:
        virtual ~RendererFactory(){}
        virtual CBaseRenderer *Create() = 0;
    };

    template <typename T>
    class TplRendererFactory : public RendererFactory
    {
    public:
        virtual CBaseRenderer *Create() override
        {
            return new T;
        }
    };
}

#endif