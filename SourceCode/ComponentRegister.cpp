#include "stdafx.h"
#include "Utility/BeatsUtility/ComponentSystem/ComponentPublic.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentManager.h"
#include "Utility/BeatsUtility/Serializer.h"
#include "Utility/BeatsUtility/EnumStrGenerator.h"
#include "Resource/ResourcePathManager.h"
#include "Render/Model.h"
#include "Render/Skeleton.h"
#include "Render/RenderState.h"
#include "Render/Material.h"

#include "Render/RenderStateParam/RenderStateParamBase.h"
#include "Render/RenderStateParam/BoolRenderStateParam.h"
#include "Render/RenderStateParam/UintRenderStateParam.h"
#include "Render/RenderStateParam/IntRenderStateParam.h"
#include "Render/RenderStateParam/FloatRenderStateParam.h"
#include "Render/RenderStateParam/FunctionRenderStateParam.h"
#include "Render/RenderStateParam/BlendRenderStateParam.h"
#include "Render/RenderStateParam/BlendEquationRenderStateParam.h"
#include "Render/RenderStateParam/CullModeRenderStateParam.h"
#include "Render/RenderStateParam/ClockWiseRenderStateParam.h"
#include "Render/RenderStateParam/PolygonModeRenderStateParam.h"
#include "Render/RenderStateParam/ShadeModeRenderStateParam.h"
#include "Render/RenderStateParam/StencilRenderStateParam.h"

#include "GUI/Window/Window.h"
#include "GUI/Window/Button.h"
#include "GUI/Window/CheckBox.h"
#include "GUI/Window/ComboBox.h"
#include "GUI/Window/ListBox.h"
#include "GUI/Window/ListControl.h"
#include "GUI/Window/Progress.h"
#include "GUI/Window/Slider.h"
#include "GUI/Window/TextEdit.h"
#include "GUI/Window/TextLabel.h"

#include "ParticlesSystem/ParticleSystem.h"
#include "ParticlesSystem/Emitter.h"
#include "ParticlesSystem/ParticleProperty.h"
#include "ParticlesSystem/BoxParticleEmitterEntity.h"
#include "ParticlesSystem/BallParticleEmitterEntity.h"
#include "ParticlesSystem/PointParticleEmitterEntity.h"
#include "ParticlesSystem/ConeParticleEmitterEntity.h"
#include "ParticlesSystem/ColorParticleAnimation.h"
#include "ParticlesSystem/RotateParticleAnimation.h"
#include "ParticlesSystem/ScaleParticleAnimation.h"
#include "ParticlesSystem/ParticleEmitterEntity.h"

#include "SkeletalAnimation/CAnimationModel.h"
//TankWar
#include "TankWar/TankComponent/Tank.h"
#include "TankWar/Skill/Skill.h"

void ForceReferenceSymbolForComponent()
{
    // When you are writing in a lib project, this cpp may not be referenced by the exe project.
    // so we add this empty function and make it extern in componentpublic.h, every cpp in the exe project,
    // which contain the componentpublic.h file will reference this symbol.
}

START_REGISTER_COMPONENT
REGISTER_COMPONENT(CModel, _T("ģ��"), _T("Render\\CModel"))
REGISTER_COMPONENT(CSkeleton, _T("����"), _T("Render\\CSkeleton"))
REGISTER_COMPONENT(CMaterial, _T("����"), _T("Render\\CMaterial"))
REGISTER_COMPONENT(CRenderState, _T("��Ⱦ״̬"), _T("Render\\CRenderState"))

REGISTER_COMPONENT(_2DSkeletalAnimation::CAnimationModel,_T("CAnimationModel"),_T("2DSkeletalAnimation\\CAnimationModel"));

REGISTER_COMPONENT(CBoolRenderStateParam, _T("CBoolRenderStateParam"), _T("Render\\RenderStateParam"))
REGISTER_COMPONENT(CUintRenderStateParam, _T("CUintRenderStateParam"), _T("Render\\RenderStateParam"))
REGISTER_COMPONENT(CIntRenderStateParam, _T("CIntRenderStateParam"), _T("Render\\RenderStateParam"))
REGISTER_COMPONENT(CFloatRenderStateParam, _T("CFloatRenderStateParam"), _T("Render\\RenderStateParam"))
REGISTER_COMPONENT(CFunctionRenderStateParam, _T("CFunctionRenderStateParam"), _T("Render\\RenderStateParam"))
REGISTER_COMPONENT(CBlendRenderStateParam, _T("CBlendRenderStateParam"), _T("Render\\RenderStateParam"))
REGISTER_COMPONENT(CBlendEquationRenderStateParam, _T("CBlendEquationParam"), _T("Render\\RenderStateParam"))
REGISTER_COMPONENT(CCullModeRenderStateParam, _T("CCullModeParam"), _T("Render\\RenderStateParam"))
REGISTER_COMPONENT(CClockWiseRenderStateParam, _T("CClockWiseParam"), _T("Render\\RenderStateParam"))
REGISTER_COMPONENT(CPolygonModeRenderStateParam, _T("CPolygonModeParam"), _T("Render\\RenderStateParam"))
REGISTER_COMPONENT(CShadeModeRenderStateParam, _T("CShadeModeParam"), _T("Render\\RenderStateParam"))
REGISTER_COMPONENT(CStencilRenderStateParam, _T("CStencilParam"), _T("Render\\RenderStateParam"))
REGISTER_ABSTRACT_COMPONENT(CRenderStateParamBase)

REGISTER_COMPONENT(FCGUI::CWindow, _T("Window"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CButton, _T("Button"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CCheckBox, _T("Check"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CListBox, _T("ListBox"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CComboBox, _T("ComboBox"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CProgress, _T("Progress"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CSlider, _T("Slider"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CTextLabel, _T("TextLabel"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CTextEdit, _T("TextEdit"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CListControl, _T("ListControl"), _T("GUI\\Window"))

REGISTER_COMPONENT(FCEngine::CParticleSystem, _T("����ϵͳ"), _T("�������"))
REGISTER_COMPONENT(FCEngine::CParticleEmitter, _T("������"), _T("�������") )
REGISTER_COMPONENT(FCEngine::CPointParticleEmitterEntity, _T("�㷢����"), _T("�������\\����������") )
REGISTER_COMPONENT(FCEngine::CBoxParticleEmitterEntity, _T("���ӷ�����"), _T("�������\\����������") )
REGISTER_COMPONENT(FCEngine::CBallParticleEmitterEntity, _T("������"), _T("�������\\����������") )
REGISTER_COMPONENT(FCEngine::CConeParticleEmitterEntity, _T("׶������"), _T("�������\\����������") )
REGISTER_COMPONENT(FCEngine::CColorParticleAnimation, _T("��ɫ����"), _T("�������\\���Ӷ�������") )
REGISTER_COMPONENT(FCEngine::CScaleParticleAnimation, _T("���Ŷ���"), _T("�������\\���Ӷ�������") )
REGISTER_COMPONENT(FCEngine::CRotateParticleAnimation, _T("��ת����"), _T("�������\\���Ӷ�������") )
REGISTER_COMPONENT(FCEngine::CParticleProperty, _T("��������"), _T("�������") )
REGISTER_ABSTRACT_COMPONENT(FCEngine::CParticleEmitterEntityBase)

REGISTER_COMPONENT(CShaderUniform, _T("CShaderUniform"), _T("CShaderUniform"))
REGISTER_COMPONENT(CShaderUniformData, _T("CShaderUniformData"), _T("CShaderUniformData"))
REGISTER_ABSTRACT_COMPONENT(FCEngine::CParticleAnimationBase)

REGISTER_COMPONENT(CTank, _T("CTank"),_T("TankWar"))
REGISTER_COMPONENT(CTankComponentBase, _T("CTankComponentBase"),_T("TankWar"))

REGISTER_COMPONENT(CSkill, _T("CSkill"),_T("TankWar"))
REGISTER_COMPONENT(CBullet, _T("CBullet"),_T("TankWar"))


END_REGISTER_COMPONENT