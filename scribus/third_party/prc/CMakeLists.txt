INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${OPENGL_INCLUDE_DIR}
${FREETYPE_INCLUDE_DIRS}
)

SET(SCRIBUS_PRC_LIB_SOURCES
    oPRCFile.cc
    PRCbitStream.cc
    PRCdouble.cc
    writePRC.cc
    exportPRC.cpp
)

SET(SCRIBUS_PRC_LIB "scribus_prc_lib")
ADD_LIBRARY(${SCRIBUS_PRC_LIB} STATIC ${SCRIBUS_PRC_LIB_SOURCES})

# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
SET_TARGET_PROPERTIES(${SCRIBUS_PRC_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )
