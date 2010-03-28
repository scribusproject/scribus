INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)
SET(LIB_TYPE STATIC)
IF (ARCH_X86_64 EQUAL 1)
  SET(CMAKE_CXX_FLAGS "$(CMAKE_CXX_FLAGS) -fPIC")
ENDIF (ARCH_X86_64 EQUAL 1)
SET(WPG_SOURCES
    WPG1Parser.cpp
    WPG2Parser.cpp
    WPGBinaryData.cpp
    WPGBitmap.cpp
    WPGBrush.cpp
    WPGColor.cpp
    WPGGradient.cpp
    WPGHeader.cpp
    WPGInternalStream.cpp
    WPGOLEStream.cpp
    WPGPath.cpp
    WPGPen.cpp
    WPGPoint.cpp
    WPGraphics.cpp
    WPGRect.cpp
    WPGStreamImplementation.cpp
    WPGString.cpp
    WPGXParser.cpp
  )

SET(SCRIBUS_WPG_LIB "scribus_wpg_lib")
ADD_LIBRARY(${SCRIBUS_WPG_LIB} STATIC ${WPG_SOURCES})

# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
SET_TARGET_PROPERTIES(${SCRIBUS_WPG_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )
