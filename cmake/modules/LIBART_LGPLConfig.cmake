#
# Find the native LIBART includes and library
#

# This module defines
# LIBART_LGPL_INCLUDE_DIR, where to find art*.h etc
# LIBART_LGPL_LIBRARY, the libraries to link against to use LIBART.
# LIBART_LGPL_FOUND, If false, do not try to use LIBART.
# LIBART_LGPL_LIBS, link information
# LIBART_LGPL_CFLAGS, cflags for include information


INCLUDE(UsePkgConfig)

# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
PKGCONFIG(libart-2.0 _libArtIncDir _libArtLinkDir _libArtLinkFlags _libArtCflags)

SET(LIBART_LGPL_LIBS ${_libArtCflags})

FIND_PATH(LIBART_LGPL_INCLUDE_DIR libart_lgpl/libart.h
${_libArtIncDir}
/usr/include
/usr/local/include/
PATH_SUFFIXES
libart-2.0
)

FIND_LIBRARY(LIBART_LGPL_LIBRARY NAMES art_lgpl_2
PATHS
${_libArtLinkDir}
/usr/lib
/usr/local/lib
)

SET(LIBART_LGPL_FIND_QUIETLY 1)

IF (LIBART_LGPL_LIBRARY)
  IF (LIBART_LGPL_INCLUDE_DIR)
    SET( LIBART_LGPL_FOUND "YES" )
    IF(NOT LIBART_LGPL_FIND_QUIETLY)
      MESSAGE(STATUS "Found Libart_LGPL: ${LIBART_LGPL_LIBRARY}")
    ENDIF(NOT LIBART_LGPL_FIND_QUIETLY)
    SET( LIBART_LGPL_LIBRARIES ${LIBART_LGPL_LIBRARY} )
    FIND_PROGRAM(LIBART_LGPL_CONFIG NAMES libart2-config PATHS ${prefix}/bin ${exec_prefix}/bin /usr/local/bin /opt/local/bin /usr/bin /usr/nekoware/bin /usr/X11/bin)
    EXEC_PROGRAM(${LIBART_LGPL_CONFIG} ARGS --libs OUTPUT_VARIABLE LIBART_LGPL_LIBS)
    EXEC_PROGRAM(${LIBART_LGPL_CONFIG} ARGS --cflags OUTPUT_VARIABLE LIBART_LGPL_CFLAGS)
  ENDIF (LIBART_LGPL_INCLUDE_DIR)
ENDIF (LIBART_LGPL_LIBRARY)