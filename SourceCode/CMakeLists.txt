set(ProjectName FCEngine)

if(WIN32)

ADD_DEFINITIONS(-DUNICODE -D_UNICODE)

set(PLATFORM_FOLDER win32)

elseif(APPLE)

set(PLATFORM_FOLDER mac)

else()

set(PLATFORM_FOLDER linux)

endif()

FILE(GLOB FCENGINE_HEADER
	./*.h
	../stdafx.h
	../targetver.h
)
FILE(GLOB FCENGINE_SRC
	./*.cpp
)

FILE(GLOB BEATSUTILITY_HEADER
	Utility/BeatsUtility/*.h
	Utility/BeatsUtility/*.hpp)
FILE(GLOB BEATSUTILITY_COMPONENTSYSTEM_HEADER
	Utility/BeatsUtility/ComponentSystem/*.h
)
FILE(GLOB BEATSUTILITY_COMPONENTSYSTEM_COMPONENT_HEADER
	Utility/BeatsUtility/ComponentSystem/Component/*.h
)
FILE(GLOB BEATSUTILITY_COMPONENTSYSTEM_DEPENDENCY_HEADER
	Utility/BeatsUtility/ComponentSystem/Dependency/*.h
)
FILE(GLOB BEATSUTILITY_COMPONENTSYSTEM_PROPERTY_HEADER
	Utility/BeatsUtility/ComponentSystem/Property/*.h
)

FILE(GLOB BEATSUTILITY_TINYXML_HEADER
	Utility/BeatsUtility/Utility/TinyXML/*.h
)

FILE(GLOB RENDER_HEADER Render/*.h)
FILE(GLOB RENDER_SRC Render/*.cpp)

FILE(GLOB RENDER_RENDERSTATEPARAM_HEADER Render/RenderStateParam/*.h)
FILE(GLOB RENDER_RENDERSTATEPARAM_SRC Render/RenderStateParam/*.cpp)

FILE(GLOB GUI_HEADER GUI/*.h)
FILE(GLOB GUI_SRC GUI/*.cpp)

FILE(GLOB GUI_ANIMATION_HEADER GUI/Animation/*.h)
FILE(GLOB GUI_ANIMATION_SRC GUI/Animation/*.cpp)

FILE(GLOB GUI_BEHAVIOR_HEADER GUI/Animation/*.h)
FILE(GLOB GUI_BEHAVIOR_SRC GUI/Animation/*.cpp)

FILE(GLOB GUI_FONT_HEADER GUI/Font/*.h)
FILE(GLOB GUI_FONT_SRC GUI/Font/*.cpp)

FILE(GLOB GUI_LAYOUT_HEADER GUI/Layout/*.h)
FILE(GLOB GUI_LAYOUT_SRC GUI/Layout/*.cpp)

FILE(GLOB GUI_GUIRENDERER_HEADER GUI/Renderer/*.h)
FILE(GLOB GUI_GUIRENDERER_SRC GUI/Renderer/*.cpp)

FILE(GLOB GUI_WINDOW_HEADER GUI/Window/*.h)
FILE(GLOB GUI_WINDOW_SRC GUI/Window/*.cpp)

FILE(GLOB PARTICLESYSTER_HEADER
	ParticlesSystem/ParticleSystem.h
  ParticlesSystem/ParticleProperty.h
  ParticlesSystem/Particle.h
  ParticlesSystem/Emitter.h
)
FILE(GLOB PARTICLESYSTER_SRC
	ParticlesSystem/ParticleSystem.cpp
  ParticlesSystem/ParticleProperty.cpp
  ParticlesSystem/Particle.cpp
  ParticlesSystem/Emitter.cpp
)

FILE(GLOB PARTICLESYSTEM_EMITTERENTITY_HEADER
	ParticlesSystem/BallParticleEmitterEntity.h
  ParticlesSystem/BoxParticleEmitterEntity.h
  ParticlesSystem/ConeParticleEmitterEntity.h
  ParticlesSystem/ParticleEmitterEntity.h
  ParticlesSystem/PointParticleEmitterEntity.h
)
FILE(GLOB PARTICLESYSTEM_EMITTERENTITY_SRC
	ParticlesSystem/BallParticleEmitterEntity.cpp
  ParticlesSystem/BoxParticleEmitterEntity.cpp
  ParticlesSystem/ConeParticleEmitterEntity.cpp
  ParticlesSystem/ParticleEmitterEntity.cpp
  ParticlesSystem/PointParticleEmitterEntity.cpp
)

FILE(GLOB PARTICLESYSTEM_PARTICLEANIMATION_HEADER
	ParticlesSystem/ColorParticleAnimation.h
  ParticlesSystem/ParticleAnimationBase.h
  ParticlesSystem/RotateParticleAnimation.h
  ParticlesSystem/ScaleParticleAnimation.h
)
FILE(GLOB PARTICLESYSTEM_PARTICLEANIMATION_SRC
	ParticlesSystem/ColorParticleAnimation.cpp
  ParticlesSystem/ParticleAnimationBase.cpp
  ParticlesSystem/RotateParticleAnimation.cpp
  ParticlesSystem/ScaleParticleAnimation.cpp
)

FILE(GLOB RESOURCE_HEADER Resource/*.h)
FILE(GLOB RESOURCE_SRC Resource/*.cpp)

FILE(GLOB SPLINE_HEADER Spline/*.h)
FILE(GLOB SPLINE_SRC Spline/*.cpp)

FILE(GLOB EVENT_HEADER Event/*.h)
FILE(GLOB EVENT_SRC Event/*.cpp)

set(ALL_HEADER
	${FCENGINE_HEADER}
	${BEATSUTILITY_HEADER}
	${BEATSUTILITY_COMPONENTSYSTEM_HEADER}
	${BEATSUTILITY_COMPONENTSYSTEM_COMPONENT_HEADER}
	${BEATSUTILITY_COMPONENTSYSTEM_DEPENDENCY_HEADER}
	${BEATSUTILITY_COMPONENTSYSTEM_PROPERTY_HEADER}
	${RENDER_HEADER}
	${GUI_HEADER}
	${GUI_ANIMATION_HEADER}
	${GUI_BEHAVIOR_HEADER}
	${GUI_FONT_HEADER}
	${GUI_LAYOUT_HEADER}
	${GUI_GUIRENDERER_HEADER}
	${GUI_WINDOW_HEADER}
	${PARTICLESYSTER_HEADER}
	${PARTICLESYSTEM_EMITTERENTITY_HEADER}
	${PARTICLESYSTEM_PARTICLEANIMATION_HEADER}
	${RESOURCE_HEADER}
	${SPLINE_HEADER}
	${EVENT_HEADER}
)

set(ALL_SRC
	${FCENGINE_SRC}
	${BEATSUTILITY_SRC}
	${BEATSUTILITY_COMPONENTSYSTEM_SRC}
	${BEATSUTILITY_COMPONENTSYSTEM_COMPONENT_SRC}
	${BEATSUTILITY_COMPONENTSYSTEM_DEPENDENCY_SRC}
	${BEATSUTILITY_COMPONENTSYSTEM_PROPERTY_SRC}
	${RENDER_SRC}
	${GUI_SRC}
	${GUI_ANIMATION_SRC}
	${GUI_BEHAVIOR_SRC}
	${GUI_FONT_SRC}
	${GUI_LAYOUT_SRC}
	${GUI_GUIRENDERER_SRC}
	${GUI_WINDOW_SRC}
	${PARTICLESYSTER_SRC}
	${PARTICLESYSTEM_EMITTERENTITY_SRC}
	${PARTICLESYSTEM_PARTICLEANIMATION_SRC}
	${RESOURCE_SRC}
	${SPLINE_SRC}
	${EVENT_SRC}
)



source_group("Header Files\\Event" FILES ${EVENT_HEADER})
source_group("Source Files\\Event" FILES ${EVENT_SRC})

source_group("Header Files\\Utility\\BeatsUtility" FILES ${BEATSUTILITY_HEADER})
source_group("Header Files\\Utility\\BeatsUtility\\ComponentSystem" FILES ${BEATSUTILITY_COMPONENTSYSTEM_HEADER})
source_group("Header Files\\Utility\\BeatsUtility\\ComponentSystem\\Component" FILES ${BEATSUTILITY_COMPONENTSYSTEM_COMPONENT_HEADER})
source_group("Header Files\\Utility\\BeatsUtility\\ComponentSystem\\Dependency" FILES ${BEATSUTILITY_COMPONENTSYSTEM_DEPENDENCY_HEADER})
source_group("Header Files\\Utility\\BeatsUtility\\ComponentSystem\\Property" FILES ${BEATSUTILITY_COMPONENTSYSTEM_PROPERTY_HEADER})
source_group("Header Files\\Utility\\TinyXML" FILES ${BEATSUTILITY_HEADER})

source_group("Header Files\\GUI" FILES ${GUI_HEADER})
source_group("Source Files\\GUI" FILES ${GUI_HEADER})
source_group("Header Files\\GUI\\Animation" FILES ${GUI_ANIMATION_HEADER})
source_group("Source Files\\GUI\\Animation" FILES ${GUI_ANIMATION_SRC})
source_group("Header Files\\GUI\\Behavior" FILES ${GUI_BEHAVIOR_HEADER})
source_group("Source Files\\GUI\\Behavior" FILES ${GUI_BEHAVIOR_SRC})
source_group("Header Files\\GUI\\Event" FILES ${GUI_EVENT_HEADER})
source_group("Source Files\\GUI\\Event" FILES ${GUI_EVENT_SRC})
source_group("Header Files\\GUI\\Font" FILES ${GUI_FONT_HEADER})
source_group("Source Files\\GUI\\Font" FILES ${GUI_FONT_SRC})
source_group("Header Files\\GUI\\Layout" FILES ${GUI_LAYOUT_HEADER})
source_group("Source Files\\GUI\\Layout" FILES ${GUI_LAYOUT_SRC})
source_group("Header Files\\GUI\\Renderer" FILES ${GUI_GUIRENDERER_HEADER})
source_group("Source Files\\GUI\\Renderer" FILES ${GUI_GUIRENDERER_SRC})
source_group("Header Files\\GUI\\Window" FILES ${GUI_WINDOW_HEADER})
source_group("Source Files\\GUI\\Window" FILES ${GUI_WINDOW_SRC})

source_group("Header Files\\ParticleSystem" FILES ${PARTICLESYSTER_HEADER})
source_group("Source Files\\ParticleSystem" FILES ${PARTICLESYSTER_SRC})
source_group("Header Files\\ParticleSystem\\EmitterEntity" FILES ${PARTICLESYSTEM_EMITTERENTITY_HEADER})
source_group("Source Files\\ParticleSystem\\EmitterEntity" FILES ${PARTICLESYSTEM_EMITTERENTITY_SRC})
source_group("Header Files\\ParticleSystem\\ParticleAnimation" FILES ${PARTICLESYSTEM_PARTICLEANIMATION_HEADER})
source_group("Source Files\\ParticleSystem\\ParticleAnimation" FILES ${PARTICLESYSTEM_PARTICLEANIMATION_SRC})

source_group("Header Files\\Render" FILES ${RENDER_HEADER})
source_group("Source Files\\Render" FILES ${RENDER_SRC})
source_group("Header Files\\Render\\RenderStateParam" FILES ${RENDER_RENDERSTATEPARAM_HEADER})
source_group("Source Files\\Render\\RenderStateParam" FILES ${RENDER_RENDERSTATEPARAM_SRC})

source_group("Header Files\\Resource" FILES ${RESOURCE_HEADER})
source_group("Source Files\\Resource" FILES ${RESOURCE_SRC})

source_group("Header Files\\Spline" FILES ${SPLINE_HEADER})
source_group("Source Files\\Spline" FILES ${SPLINE_SRC})


add_library(${ProjectName} STATIC
  ${ALL_HEADER}
  ${ALL_SRC}
  ../stdafx.cpp
)
    
if(WIN32)
  ADD_DEFINITIONS (-DWIN32 -D_WINDOWS)
  
  if(MSVC)
  	ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS 
  				-D_CRT_NON_CONFORMING_SWPRINTFS 
  				-D_SCL_SECURE_NO_WARNINGS
  				-wd4100 -wd4127)

  	INCLUDE(MSVCPCH.cmake)
  	USE_MSVC_PCH(${ProjectName} ../stdafx.h ../stdafx.cpp)
  else(MSVC)
  
  endif(MSVC)
elseif(APPLE)

else()#Linux
ADD_DEFINITIONS(-DLINUX)
endif()

include_directories(
	..
	../../cocos2d-x-3.0rc1/cocos/2d/platform
	../../cocos2d-x-3.0rc1/cocos/2d/platform/${PLATFORM_FOLDER}
	../../cocos2d-x-3.0rc1/cocos/2d
	../../cocos2d-x-3.0rc1/cocos
	../../cocos2d-x-3.0rc1/cocos/base
	../../cocos2d-x-3.0rc1/external/glfw3/include/${PLATFORM_FOLDER}
	../../cocos2d-x-3.0rc1/external/freetype2/include/${PLATFORM_FOLDER}
	../../cocos2d-x-3.0rc1/external/${PLATFORM_FOLDER}-specific/gles/include/OGLES
	../Lib
	../Lib/zlib
	../Lib/boost
	../SourceCode
	
)


if(WIN32)

  if(NOT MINGW)
  link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/Beats
  )
  endif()

elseif(APPLE)

else()

endif()

set_target_properties(ProjectName
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib.${PLATFORM_FOLDER}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib.${PLATFORM_FOLDER}"
)

