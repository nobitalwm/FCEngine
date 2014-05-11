cmake_minimum_required(VERSION 2.8)

project (FCEngine_v1.0)


if(DEBUG_MODE)
  set(CMAKE_BUILD_TYPE DEBUG)
else(DEBUG_MODE)
  set(CMAKE_BUILD_TYPE RELEASE)
endif(DEBUG_MODE)


# architecture
if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
set(ARCH_DIR "64-bit")
else()
set(ARCH_DIR "32-bit")
endif()

if(WIN32)

  if(NOT MINGW)
  link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/Beats
  )
  endif()

elseif(APPLE)

else()

endif()

add_subdirectory(SourceCode)