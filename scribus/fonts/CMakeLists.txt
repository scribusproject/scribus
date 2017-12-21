include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${FREETYPE_INCLUDE_DIRS}
${HARFBUZZ_INCLUDE_DIRS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_FPIC}")

set(SCRIBUS_FONTS_LIB_SOURCES
  cff.cpp
  fontfeatures.cpp
  ftface.cpp
  scface.cpp
  scface_ps.cpp
  scface_ttf.cpp
  scfontmetrics.cpp
  sfnt.cpp
)
set(SCRIBUS_FONTS_LIB "scribus_fonts_lib")
add_library(${SCRIBUS_FONTS_LIB} STATIC ${SCRIBUS_FONTS_LIB_SOURCES})
# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
set_target_properties(${SCRIBUS_FONTS_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )
