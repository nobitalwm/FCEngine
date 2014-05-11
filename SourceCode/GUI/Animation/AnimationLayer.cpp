#include "stdafx.h"
#include "AnimationLayer.h"
#include "KeyFrame.h"
#include "Animatable.h"
#include "AnimProp.h"
#include "Animation_ui.h"

using namespace FCGUI;

CAnimationLayer::CAnimationLayer(const TString &name)
    : m_strName(name)
    , m_pAnimation(nullptr)
    , m_bEnabled(true)
{

}

CAnimationLayer::~CAnimationLayer()
{
    for(auto keyFrame : m_keyFrames)
    {
        BEATS_SAFE_DELETE(keyFrame);
    }
}

void CAnimationLayer::Apply(CAnimatable *target, kmScalar position)
{
    if(m_bEnabled && !m_keyFrames.empty())
    {
        CKeyFrame *left = nullptr;
        CKeyFrame *right = m_keyFrames.front();

        for(size_t i = 0; i < m_keyFrames.size(); ++i)
        {
            CKeyFrame *curr = m_keyFrames[i];
            if(position > curr->Position())
            {
                left = curr;
                if((i + 1) < m_keyFrames.size())
                {
                    right = m_keyFrames[i + 1];
                    if(position <= right->Position())
                        break;
                }
                else
                    right = nullptr;
            }
        }

        CKeyFrame::TValueList values = interpolation(left, right, position);
        BEATS_ASSERT(values.size() == m_propIDs.size());
        for (size_t i = 0; i < m_propIDs.size(); i++)
        {
            const CAnimProp *prop = target->GetProp(m_propIDs[i]);
            BEATS_ASSERT(prop);
            prop->m_setter(values[i]);
        }
    }
}

CKeyFrame::TValueList CAnimationLayer::interpolation(const CKeyFrame *left, const CKeyFrame *right, 
                                                  kmScalar position)
{
    CKeyFrame::TValueList values;
    if(!left && right)
    {
        BEATS_ASSERT(right->ValueCount() == m_propIDs.size());
        values = right->Values();
    }
    else if(!right && left)
    {
        BEATS_ASSERT(left->ValueCount() == m_propIDs.size());
        values = left->Values();
    }
    else if(left && right)
    {
        BEATS_ASSERT(left->ValueCount() == m_propIDs.size());
        BEATS_ASSERT(right->ValueCount() == m_propIDs.size());
        kmScalar percent = (position - left->Position()) / (right->Position() - left->Position());
        for(size_t i = 0; i < left->ValueCount(); ++i)
        {
            kmScalar value = left->Value(i) * (1 - percent) + right->Value(i) * percent;
            values.push_back(value);
        }
    }
    return values;
}

void CAnimationLayer::SetAnimation(CAnimation *animation)
{
    m_pAnimation = animation;
}

void CAnimationLayer::SetEnabled(bool enabled)
{
    m_bEnabled = enabled;
}

bool CAnimationLayer::Enabled() const
{
    return m_bEnabled;
}

void CAnimationLayer::AddPropID(int propID)
{
    m_propIDs.push_back(propID);
    for(auto keyFrame : m_keyFrames)
    {
        keyFrame->SetValueCount(m_propIDs.size());
    }
}

void CAnimationLayer::DeletePropID(int propID)
{
    for(size_t i = 0; i < m_propIDs.size(); ++i)
    {
        if(m_propIDs[i] == propID)
        {
            m_propIDs.erase(m_propIDs.begin() + i);
            for (auto keyFrame : m_keyFrames)
            {
                keyFrame->DeleteValue(i);
            }
            break;
        }
    }
}

CKeyFrame *CAnimationLayer::CreateKeyFrame(kmScalar position)
{
    BEATS_ASSERT(m_pAnimation);
    position = std::min(position, m_pAnimation->Duration());
    CKeyFrame *keyFrame = new CKeyFrame(position);
    keyFrame->SetValueCount(m_propIDs.size());
    m_keyFrames.push_back(keyFrame);
    return keyFrame;
}

void CAnimationLayer::DeleteKeyFrame(CKeyFrame *keyFrame)
{
    for(size_t i = 0; i < m_keyFrames.size(); ++i)
    {
        if(m_keyFrames[i] == keyFrame)
        {
            m_keyFrames.erase(m_keyFrames.begin() + i);
            BEATS_SAFE_DELETE(keyFrame);
            break;
        }
    }
}
