include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${OPENGL_INCLUDE_DIR}
${FREETYPE_INCLUDE_DIRS}
)

set(SCRIBUS_PRC_LIB_SOURCES
    oPRCFile.cc
    PRCbitStream.cc
    PRCdouble.cc
    writePRC.cc
    exportPRC.cpp
)

set(SCRIBUS_PRC_LIB "scribus_prc_lib")
add_library(${SCRIBUS_PRC_LIB} STATIC ${SCRIBUS_PRC_LIB_SOURCES})

# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
set_target_properties(${SCRIBUS_PRC_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )
