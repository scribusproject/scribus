#
# Find the native LCMS includes and library
#

# This module defines
# LCMS_INCLUDE_DIR, where to find art*.h etc
# LCMS_LIBRARY, the libraries
# LCMS_FOUND, If false, do not try to use LCMS.
# LIBLCMS_LIBS, link information
# LIBLCMS_CFLAGS, cflags for include information


# include(UsePkgConfig)

# use pkg-config to get the directories and then use these values
# in the find_path() and find_library() calls
# PKGCONFIG(lcms _lcmsIncDir _lcmsLinkDir _lcmsLinkFlags _lcmsCflags)

# set(LCMS2_LIBS ${_lcmsCflags})

find_path(LCMS2_INCLUDE_DIR lcms2.h
  /usr/include
  /usr/local/include
  PATH_SUFFIXES lcms2
)

find_library(LCMS2_LIBRARY_RELEASE
  NAMES ${LCMS2_NAMES_RELEASE} ${LCMS2_NAMES} lcms2 liblcms2 lcms2dll
  PATHS /usr/lib /usr/local/lib
)

find_library(LCMS2_LIBRARY_DEBUG
  NAMES ${LCMS2_NAMES_DEBUG} lcms2d liblcms2d lcms2dlld
  PATHS /usr/lib /usr/local/lib
)

include(LibraryDebugAndRelease)
SET_LIBRARY_FROM_DEBUG_AND_RELEASE(LCMS2)

message("LCMS 2 ReleaseLibrary: ${LCMS2_LIBRARY_RELEASE}")
message("LCMS 2 Debug Library: ${LCMS2_LIBRARY_DEBUG}")
message("LCMS 2 Library: ${LCMS2_LIBRARY}")

if (LCMS2_LIBRARY AND LCMS2_INCLUDE_DIR)
  set( LCMS2_FOUND 1 )
  set( LCMS2_LIBRARIES ${LCMS2_LIBRARY} )
else (LCMS2_LIBRARY AND LCMS2_INCLUDE_DIR)
  set( LCMS2_FOUND 0 )
endif (LCMS2_LIBRARY AND LCMS2_INCLUDE_DIR)

set(LCMS2_FIND_QUIETLY 1)

if (LCMS2_FOUND)
  if (NOT LCMS2_FIND_QUIETLY)
	message(STATUS "Found LittleCMS 2: ${LCMS2_LIBRARY}")
  endif (NOT LCMS2_FIND_QUIETLY)
else (LCMS2_FOUND)
  if (LCMS2_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find LittleCMS")
  endif (LCMS2_FIND_REQUIRED)
endif (LCMS2_FOUND)
