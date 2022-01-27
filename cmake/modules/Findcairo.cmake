#
# Find the native cairo includes and library
#

# This module defines
# cairo_INCLUDE_DIR, where to find art*.h etc
# cairo_LIBRARY, the libraries to link against to use LIBART.
# cairo_FOUND, If false, do not try to use LIBART.
# cairo_LIBS, link information
# cairo_CFLAGS, cflags for include information


if(PREFIX_CAIRO)
  message(STATUS "Searching custom cairo location: ${PREFIX_CAIRO}")
  set(PREFIX_cairo_INCLUDE "${PREFIX_CAIRO}/include")
  set(PREFIX_cairo_LIB "${PREFIX_CAIRO}/lib")
  set(PREFIX_cairo_BIN "${PREFIX_CAIRO}/bin")
else(PREFIX_CAIRO)
  # use pkgconfig to get the directories and then use these values
  # in the find_path() and find_library() calls
  #include(FindPkgConfig)
  find_package(PkgConfig QUIET)
  pkg_search_module(cairo REQUIRED libcairo>=1.14.0 cairo>=1.14.0)
endif(PREFIX_CAIRO)

find_path(cairo_INCLUDE_DIR
  NAMES cairo.h
  PATHS ${PREFIX_cairo_INCLUDE} ${PKG_cairo_INCLUDE_DIRS} ${cairo_INCLUDE_DIRS}
  PATH_SUFFIXES cairo
  NO_DEFAULT_PATH
)

find_library(cairo_LIBRARY
  NAMES libcairo cairo
  PATHS ${PREFIX_cairo_LIB} ${PKG_cairo_LIBRARIES} ${cairo_LIBRARY_DIRS}
  NO_DEFAULT_PATH
)

#If we had no cairo prefix specify it, set PREFIX_cairo_LIB most importantly, for the
# if(NOT PREFIX_CAIRO)
# 	get_filename_component(cairo_DIR ${cairo_LIBRARY} PATH)
# 	get_filename_component(cairo_DIR ${cairo_DIR} PATH)
# 	set(PREFIX_CAIRO ${cairo_DIR})
# 	if(PREFIX_CAIRO)
# 		set(PREFIX_cairo_INCLUDE "${PREFIX_CAIRO}/include")
# 		set(PREFIX_cairo_LIB "${PREFIX_CAIRO}/lib")
# 		set(PREFIX_cairo_BIN "${PREFIX_CAIRO}/bin")
# 	endif(PREFIX_CAIRO)
# endif(NOT PREFIX_CAIRO)

set(cairo_FIND_QUIETLY ON)

if (cairo_LIBRARY)
  if (cairo_INCLUDE_DIR)
	set( cairo_FOUND "YES" )
	if(NOT cairo_FIND_QUIETLY)
		message(STATUS "Found cairo: ${cairo_LIBRARY}")
	endif(NOT cairo_FIND_QUIETLY)
	set( cairo_LIBRARIES ${cairo_LIBRARY} )
#     CB: We dont use this yet anyway
#     find_program(cairo_CONFIG
#       NAMES pkg-config
#       PATHS ${prefix}/bin ${exec_prefix}/bin /usr/local/bin /opt/local/bin /usr/bin /usr/nekoware/bin /usr/X11/bin
#     )
#     exec_program(${cairo_CONFIG} ARGS "--libs cairo" OUTPUT_VARIABLE cairo_LIBS)
#     exec_program(${cairo_CONFIG} ARGS "--cflags cairo" OUTPUT_VARIABLE cairo_CFLAGS)
  endif (cairo_INCLUDE_DIR)
endif (cairo_LIBRARY)
