include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${FREETYPE_INCLUDE_DIRS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_FPIC}")

set(SCRIBUS_DESAXE_LIB_SOURCES
saxXML.cpp
saxiohelper.cpp
saxfilter.cpp
uniqueid.cpp
digester.cpp
digester_parse.cpp
)

set(SCRIBUS_DESAXE_LIB "scribus_desaxe_lib")
add_library(${SCRIBUS_DESAXE_LIB} STATIC ${SCRIBUS_DESAXE_LIB_SOURCES})

# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
set_target_properties(${SCRIBUS_DESAXE_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )
