#ifndef GUI_WINDOW_WINDOW_FACTORY_H__INCLUDE
#define GUI_WINDOW_WINDOW_FACTORY_H__INCLUDE

namespace FCGUI
{
    class CWindow;

    class CWindowFactory
    {
    public:
        virtual ~CWindowFactory(){}
        virtual CWindow *Create(const TString &name) = 0;
    };

    template <typename T>
    class TplWindowFactory : public CWindowFactory
    {
    public:
        virtual CWindow *Create(const TString &name) override
        {
            return new T(name);
        }
    };
}

#endif