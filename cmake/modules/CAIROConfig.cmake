#
# Find the native cairo includes and library
#

# This module defines
# CAIRO_INCLUDE_DIR, where to find art*.h etc
# CAIRO_LIBRARY, the libraries to link against to use LIBART.
# CAIRO_FOUND, If false, do not try to use LIBART.
# CAIRO_LIBS, link information
# CAIRO_CFLAGS, cflags for include information


IF(PREFIX_CAIRO)
  MESSAGE(STATUS "Searching custom cairo location: ${PREFIX_CAIRO}")
  SET(PREFIX_CAIRO_INCLUDE "${PREFIX_CAIRO}/include")
  SET(PREFIX_CAIRO_LIB "${PREFIX_CAIRO}/lib")
  SET(PREFIX_CAIRO_BIN "${PREFIX_CAIRO}/bin")
ELSE(PREFIX_CAIRO)
  # use pkgconfig to get the directories and then use these values
  # in the FIND_PATH() and FIND_LIBRARY() calls
  IF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.5)
    INCLUDE(UsePkgConfig)
    PKGCONFIG(cairo _libCairoIncDir _libCairoLinkDir _libCairoLinkFlags _libCairoCflags)
    SET(PKG_CAIRO_INCLUDE_DIRS ${_libCairoIncDir})
    SET(PKG_CAIRO_LIBRARIES ${_libCairoLinkDir})
  ELSE (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.5)
    INCLUDE(FindPkgConfig)
    pkg_search_module(CAIRO REQUIRED libcairo>=1.2.0 cairo>=1.2.0)
  ENDIF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.5)
ENDIF(PREFIX_CAIRO)

FIND_PATH(CAIRO_INCLUDE_DIR 
  NAMES cairo.h
  PATHS ${PREFIX_CAIRO_INCLUDE} ${PKG_CAIRO_INCLUDE_DIRS} ${CAIRO_INCLUDE_DIRS} /usr/local/include /usr/include
  PATH_SUFFIXES cairo
  NO_DEFAULT_PATH
)

FIND_LIBRARY(CAIRO_LIBRARY
  NAMES libcairo cairo
  PATHS ${PREFIX_CAIRO_LIB} ${PKG_CAIRO_LIBRARIES} ${CAIRO_LIBRARY_DIRS} /usr/local/lib /usr/lib
  NO_DEFAULT_PATH
)

#If we had no cairo prefix specify it, set PREFIX_CAIRO_LIB most importantly, for the 
# IF(NOT PREFIX_CAIRO)
# 	GET_FILENAME_COMPONENT(CAIRO_DIR ${CAIRO_LIBRARY} PATH)
# 	GET_FILENAME_COMPONENT(CAIRO_DIR ${CAIRO_DIR} PATH)
# 	SET(PREFIX_CAIRO ${CAIRO_DIR})
# 	IF(PREFIX_CAIRO)
# 		SET(PREFIX_CAIRO_INCLUDE "${PREFIX_CAIRO}/include")
# 		SET(PREFIX_CAIRO_LIB "${PREFIX_CAIRO}/lib")
# 		SET(PREFIX_CAIRO_BIN "${PREFIX_CAIRO}/bin")
# 	ENDIF(PREFIX_CAIRO)
# ENDIF(NOT PREFIX_CAIRO)

SET(CAIRO_FIND_QUIETLY 1)

IF (CAIRO_LIBRARY)
  IF (CAIRO_INCLUDE_DIR)
    SET( CAIRO_FOUND "YES" )
    IF(NOT CAIRO_FIND_QUIETLY)
      MESSAGE(STATUS "Found cairo: ${CAIRO_LIBRARY}")
    ENDIF(NOT CAIRO_FIND_QUIETLY)
    SET( CAIRO_LIBRARIES ${CAIRO_LIBRARY} )
#     CB: We dont use this yet anyway
#     FIND_PROGRAM(CAIRO_CONFIG 
#       NAMES pkg-config
#       PATHS ${prefix}/bin ${exec_prefix}/bin /usr/local/bin /opt/local/bin /usr/bin /usr/nekoware/bin /usr/X11/bin
#     )
#     EXEC_PROGRAM(${CAIRO_CONFIG} ARGS "--libs cairo" OUTPUT_VARIABLE CAIRO_LIBS)
#     EXEC_PROGRAM(${CAIRO_CONFIG} ARGS "--cflags cairo" OUTPUT_VARIABLE CAIRO_CFLAGS)
  ENDIF (CAIRO_INCLUDE_DIR)
ENDIF (CAIRO_LIBRARY)
