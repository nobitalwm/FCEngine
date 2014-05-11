#ifndef GUI_WINDOW_WINDOWDEFS_H__INCLUDE
#define GUI_WINDOW_WINDOWDEFS_H__INCLUDE

namespace FCGUI
{
    enum EWindowType
    {
        eWT_WINDOW_BASE,
        eWT_WINDOW_BUTTON,
        eWT_WINDOW_CHECKBOX,
        eWT_WINDOW_SLIDER,
        eWT_WINDOW_PROGRESS,
        eWT_WINDOW_TEXTEDIT,
        eWT_WINDOW_TEXTLABEL,
        eWT_WINDOW_LISTBOX,
        eWT_WINDOW_COMBOBOX,
        eWT_WINDOW_LISTCONTROL,
        
        eWT_WINDOW_COUNT,
    };
}

#define DEFINE_WINDOW_TYPE(type) \
    public: \
        static const EWindowType TYPE = type;   \
        virtual EWindowType Type() const \
        {   \
            return TYPE;    \
        }   \
    private:

#endif