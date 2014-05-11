#ifndef GUI_ANIMATION_ANIMATABLE_H__INCLUDE
#define GUI_ANIMATION_ANIMATABLE_H__INCLUDE

#include "AnimProp.h"

namespace FCGUI
{
    class CAnimatable
    {
    public:
        CAnimatable();
        virtual ~CAnimatable();

        void AddProp(int ID, const TString &name, 
            const CAnimProp::Setter &setter, const CAnimProp::Getter &getter);

        const CAnimProp *GetProp(int ID) const;

    private:
        std::map<int, CAnimProp *> m_props;
    };
}

#endif