INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
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
