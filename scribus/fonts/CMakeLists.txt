INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${FREETYPE_INCLUDE_DIRS}
)


SET(SCRIBUS_FONTS_LIB_SOURCES
  scface.cpp
  ftface.cpp
  scface_ps.cpp
  scface_ttf.cpp
  scfontmetrics.cpp
)
SET(SCRIBUS_FONTS_LIB "scribus_fonts_lib")
ADD_LIBRARY(${SCRIBUS_FONTS_LIB} STATIC ${SCRIBUS_FONTS_LIB_SOURCES})
# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
SET_TARGET_PROPERTIES(${SCRIBUS_FONTS_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )
