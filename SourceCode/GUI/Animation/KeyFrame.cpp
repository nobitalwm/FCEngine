#include "stdafx.h"
#include "KeyFrame.h"

using namespace FCGUI;

CKeyFrame::CKeyFrame(kmScalar position)
    : m_position(position)
{

}

kmScalar CKeyFrame::Position() const
{
    return m_position;
}

void CKeyFrame::SetValueCount(size_t size)
{
    m_values.resize(size);
}

size_t CKeyFrame::ValueCount() const
{
    return m_values.size();
}

void CKeyFrame::SetValue(size_t index, kmScalar value)
{
    BEATS_ASSERT(index < m_values.size());
    m_values[index] = value;
}

void CKeyFrame::DeleteValue(size_t index)
{
    m_values.erase(m_values.begin() + index);
}

kmScalar CKeyFrame::Value(size_t index) const
{
    BEATS_ASSERT(index < m_values.size());
    return m_values[index];
}

const CKeyFrame::TValueList &CKeyFrame::Values() const
{
    return m_values;
}