#
# Find the native FREETYPE includes and library
#

# This module defines
# FREETYPE_INCLUDE_DIR, where to find ft2build.h, ftheader.h, ...
# FREETYPE_LIBRARIES, the libraries to link against to use FREETYPE.
# FREETYPE_FOUND, If false, do not try to use FREETYPE.

# also defined, but not for general use are
# FREETYPE_LIBRARY, where to find the FREETYPE library.

if (BUILD_OSX_BUNDLE)
  find_path(FREETYPE_INCLUDE_DIR_FT2BUILD ft2build.h
    /opt/local/include/
    NO_CMAKE_SYSTEM_PATH
  )
else (BUILD_OSX_BUNDLE)
  find_path(FREETYPE_INCLUDE_DIR_FT2BUILD ft2build.h
    /usr/include/
    /usr/local/include/
    NO_CMAKE_SYSTEM_PATH
  )
endif (BUILD_OSX_BUNDLE)


if (BUILD_OSX_BUNDLE)
  find_path(FREETYPE_INCLUDE_DIR_FTHEADER freetype/config/ftheader.h
    /opt/local/include/freetype2
    NO_CMAKE_SYSTEM_PATH
  )
else (BUILD_OSX_BUNDLE)
  find_path(FREETYPE_INCLUDE_DIR_FTHEADER freetype/config/ftheader.h
    /usr/include/freetype2
    /usr/local/include/freetype2
    NO_CMAKE_SYSTEM_PATH
  )
endif (BUILD_OSX_BUNDLE)

set(FREETYPE_LIBRARY_NAMES_DEBUG ${FREETYPE_LIBRARY_NAMES_DEBUG} freetyped)
set(FREETYPE_LIBRARY_NAMES_RELEASE ${FREETYPE_LIBRARY_NAMES_RELEASE} freetype)

if(BUILD_OSX_BUNDLE)
  set(FREETYPE_LIB_PATHS /opt/local/lib)
else(BUILD_OSX_BUNDLE)
  set(FREETYPE_LIB_PATHS /usr/lib /usr/local/lib)
endif(BUILD_OSX_BUNDLE)

find_library(FREETYPE_LIBRARY_RELEASE ${FREETYPE_LIBRARY_NAMES_RELEASE} ${FREETYPE_LIBRARY_NAMES}
  PATHS
  ${FREETYPE_LIB_PATHS}
  NO_CMAKE_SYSTEM_PATH)
find_library(FREETYPE_LIBRARY_DEBUG ${FREETYPE_LIBRARY_NAMES_DEBUG}
  PATHS
  ${FREETYPE_LIB_PATHS}
  NO_CMAKE_SYSTEM_PATH)

include(LibraryDebugAndRelease)
SET_LIBRARY_FROM_DEBUG_AND_RELEASE(FREETYPE)

# message(STATUS "ft lib ${FREETYPE_LIBRARY}")
# message(STATUS "ft2 build ${FREETYPE_INCLUDE_DIR_FT2BUILD}")
# message(STATUS "ft header ${FREETYPE_INCLUDE_DIR_FTHEADER}")

set(FREETYPE_FIND_QUIETLY 1)

if (FREETYPE_LIBRARY)
  if (NOT FREETYPE_FIND_QUIETLY)
    message(STATUS "Found Freetype2: ${FREETYPE_LIBRARY}")
  endif (NOT FREETYPE_FIND_QUIETLY)
  if (FREETYPE_INCLUDE_DIR_FTHEADER AND FREETYPE_INCLUDE_DIR_FT2BUILD)
    set( FREETYPE_FOUND "YES" )
    set( FREETYPE_INCLUDE_DIR
      ${FREETYPE_INCLUDE_DIR_FT2BUILD}
      ${FREETYPE_INCLUDE_DIR_FTHEADER} )
    set( FREETYPE_LIBRARIES ${FREETYPE_LIBRARY} )
    find_program(FREETYPE_CONFIG NAMES freetype-config PATHS ${prefix}/bin ${exec_prefix}/bin /usr/local/bin /opt/local/bin /usr/bin /usr/nekoware/bin /usr/X11/bin)
    exec_program(${FREETYPE_CONFIG} ARGS --libs OUTPUT_VARIABLE LIBFREETYPE_LIBS)
    exec_program(${FREETYPE_CONFIG} ARGS --cflags OUTPUT_VARIABLE LIBFREETYPE_CFLAGS)
  endif (FREETYPE_INCLUDE_DIR_FTHEADER AND FREETYPE_INCLUDE_DIR_FT2BUILD)
endif (FREETYPE_LIBRARY)
