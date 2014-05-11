#ifndef GUI_ANIMATION_H__INCLUDE
#define GUI_ANIMATION_H__INCLUDE

namespace FCGUI
{
    class CKeyFrame
    {
    public:
        typedef std::vector<kmScalar> TValueList;

        CKeyFrame(kmScalar position);

        kmScalar Position() const;

        void SetValueCount(size_t size);
        size_t ValueCount() const;

        void SetValue(size_t index, kmScalar value);
        void DeleteValue(size_t index);
        kmScalar Value(size_t index) const;
        const TValueList &Values() const;

    private:
        kmScalar m_position;
        TValueList m_values;
    };

}

#endif // !GUI_ANIMATION_H__INCLUDE
