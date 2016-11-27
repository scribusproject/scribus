include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_FPIC}")

set(LIB_TYPE STATIC)

set(WPG_SOURCES
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

set(SCRIBUS_WPG_LIB "scribus_wpg_lib")
add_library(${SCRIBUS_WPG_LIB} STATIC ${WPG_SOURCES})

# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
set_target_properties(${SCRIBUS_WPG_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )
