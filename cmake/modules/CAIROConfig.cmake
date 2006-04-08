#
# Find the native cairo includes and library
#

# This module defines
# CAIRO_INCLUDE_DIR, where to find art*.h etc
# CAIRO_LIBRARY, the libraries to link against to use LIBART.
# CAIRO_FOUND, If false, do not try to use LIBART.
# CAIRO_LIBS, link information
# CAIRO_CFLAGS, cflags for include information


INCLUDE(UsePkgConfig)

# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
PKGCONFIG(cairo _libCairoIncDir _libCairoLinkDir _libCairoLinkFlags _libCairoCflags)

SET(CAIRO_LIBS ${_libCairoCflags})

FIND_PATH(CAIRO_INCLUDE_DIR cairo.h
${_libCairoIncDir}
/usr/include
/usr/local/include/
PATH_SUFFIXES
cairo
)

FIND_LIBRARY(CAIRO_LIBRARY NAMES cairo
PATHS
${_libCairoLinkDir}
/usr/lib
/usr/local/lib
)

SET(CAIRO_FIND_QUIETLY 1)

IF (CAIRO_LIBRARY)
  IF (CAIRO_INCLUDE_DIR)
    SET( CAIRO_FOUND "YES" )
    IF(NOT CAIRO_FIND_QUIETLY)
      MESSAGE(STATUS "Found cairo: ${CAIRO_LIBRARY}")
    ENDIF(NOT CAIRO_FIND_QUIETLY)
    SET( CAIRO_LIBRARIES ${CAIRO_LIBRARY} )
     FIND_PROGRAM(CAIRO_CONFIG NAMES pkg-config PATHS ${prefix}/bin ${exec_prefix}/bin /usr/local/bin /opt/local/bin /usr/bin /usr/nekoware/bin /usr/X11/bin)
     EXEC_PROGRAM(${CAIRO_CONFIG} ARGS "--libs cairo" OUTPUT_VARIABLE CAIRO_LIBS)
     EXEC_PROGRAM(${CAIRO_CONFIG} ARGS "--cflags cairo" OUTPUT_VARIABLE CAIRO_CFLAGS)
  ENDIF (CAIRO_INCLUDE_DIR)
ENDIF (CAIRO_LIBRARY)