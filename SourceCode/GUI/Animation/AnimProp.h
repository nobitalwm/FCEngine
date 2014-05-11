#ifndef GUI_ANIMATION_ANIM_PROP_H__INCLUDE
#define GUI_ANIMATION_ANIM_PROP_H__INCLUDE

namespace FCGUI
{
    class CAnimProp
    {
    public:
        typedef std::function<void(kmScalar)> Setter;
        typedef std::function<kmScalar()> Getter;

        CAnimProp(int ID, const TString &name, const Setter &setter, const Getter &getter)
            : m_nID(ID)
            , m_strName(name)
            , m_setter(setter)
            , m_getter(getter)
        {}

        int m_nID;
        TString m_strName;
        Setter m_setter;
        Getter m_getter;
    };
}

#endif // !GUI_ANIMATION_ANIM_PROP_H__INCLUDE
