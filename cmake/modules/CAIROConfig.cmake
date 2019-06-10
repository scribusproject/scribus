#
# Find the native cairo includes and library
#

# This module defines
# CAIRO_INCLUDE_DIR, where to find art*.h etc
# CAIRO_LIBRARY, the libraries to link against to use LIBART.
# CAIRO_FOUND, If false, do not try to use LIBART.
# CAIRO_LIBS, link information
# CAIRO_CFLAGS, cflags for include information


if(PREFIX_CAIRO)
  message(STATUS "Searching custom cairo location: ${PREFIX_CAIRO}")
  set(PREFIX_CAIRO_INCLUDE "${PREFIX_CAIRO}/include")
  set(PREFIX_CAIRO_LIB "${PREFIX_CAIRO}/lib")
  set(PREFIX_CAIRO_BIN "${PREFIX_CAIRO}/bin")
else(PREFIX_CAIRO)
  # use pkgconfig to get the directories and then use these values
  # in the find_path() and find_library() calls
  if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.5)
    include(UsePkgConfig)
    PKGCONFIG(cairo _libCairoIncDir _libCairoLinkDir _libCairoLinkFlags _libCairoCflags)
    set(PKG_CAIRO_INCLUDE_DIRS ${_libCairoIncDir})
    set(PKG_CAIRO_LIBRARIES ${_libCairoLinkDir})
  else (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.5)
    include(FindPkgConfig)
    pkg_search_module(CAIRO REQUIRED libcairo>=1.14.0 cairo>=1.14.0)
  endif (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.5)
endif(PREFIX_CAIRO)

find_path(CAIRO_INCLUDE_DIR 
  NAMES cairo.h
  PATHS ${PREFIX_CAIRO_INCLUDE} ${PKG_CAIRO_INCLUDE_DIRS} ${CAIRO_INCLUDE_DIRS} /usr/local/include /usr/include
  PATH_SUFFIXES cairo
  NO_DEFAULT_PATH
)

find_library(CAIRO_LIBRARY
  NAMES libcairo cairo
  PATHS ${PREFIX_CAIRO_LIB} ${PKG_CAIRO_LIBRARIES} ${CAIRO_LIBRARY_DIRS} /usr/local/lib /usr/lib
  NO_DEFAULT_PATH
)

#If we had no cairo prefix specify it, set PREFIX_CAIRO_LIB most importantly, for the 
# if(NOT PREFIX_CAIRO)
# 	get_filename_component(CAIRO_DIR ${CAIRO_LIBRARY} PATH)
# 	get_filename_component(CAIRO_DIR ${CAIRO_DIR} PATH)
# 	set(PREFIX_CAIRO ${CAIRO_DIR})
# 	if(PREFIX_CAIRO)
# 		set(PREFIX_CAIRO_INCLUDE "${PREFIX_CAIRO}/include")
# 		set(PREFIX_CAIRO_LIB "${PREFIX_CAIRO}/lib")
# 		set(PREFIX_CAIRO_BIN "${PREFIX_CAIRO}/bin")
# 	endif(PREFIX_CAIRO)
# endif(NOT PREFIX_CAIRO)

set(CAIRO_FIND_QUIETLY ON)

if (CAIRO_LIBRARY)
  if (CAIRO_INCLUDE_DIR)
    set( CAIRO_FOUND "YES" )
    if(NOT CAIRO_FIND_QUIETLY)
      message(STATUS "Found cairo: ${CAIRO_LIBRARY}")
    endif(NOT CAIRO_FIND_QUIETLY)
    set( CAIRO_LIBRARIES ${CAIRO_LIBRARY} )
#     CB: We dont use this yet anyway
#     find_program(CAIRO_CONFIG 
#       NAMES pkg-config
#       PATHS ${prefix}/bin ${exec_prefix}/bin /usr/local/bin /opt/local/bin /usr/bin /usr/nekoware/bin /usr/X11/bin
#     )
#     exec_program(${CAIRO_CONFIG} ARGS "--libs cairo" OUTPUT_VARIABLE CAIRO_LIBS)
#     exec_program(${CAIRO_CONFIG} ARGS "--cflags cairo" OUTPUT_VARIABLE CAIRO_CFLAGS)
  endif (CAIRO_INCLUDE_DIR)
endif (CAIRO_LIBRARY)
