INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)


SET(SCRIBUS_DESAXE_LIB_SOURCES
saxXML.cpp
saxiohelper.cpp
saxfilter.cpp
uniqueid.cpp
digester.cpp
digester_parse.cpp
)

SET(SCRIBUS_DESAXE_LIB "scribus_desaxe_lib")
ADD_LIBRARY(${SCRIBUS_DESAXE_LIB} STATIC ${SCRIBUS_DESAXE_LIB_SOURCES})

# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
SET_TARGET_PROPERTIES(${SCRIBUS_FONTS_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )
