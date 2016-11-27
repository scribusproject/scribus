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

# set(LCMS_LIBS ${_lcmsCflags})

find_path(LCMS_INCLUDE_DIR lcms.h
  /usr/include
  /usr/local/include
  PATH_SUFFIXES lcms
)

find_library(LCMS_LIBRARY_RELEASE
  NAMES ${LCMS_NAMES_RELEASE} ${LCMS_NAMES} lcms liblcms lcmsdll
  PATHS /usr/lib /usr/local/lib
)

find_library(LCMS_LIBRARY_DEBUG
  NAMES ${LCMS_NAMES_DEBUG} lcmsd liblcmsd lcmsdlld
  PATHS /usr/lib /usr/local/lib
)

include(LibraryDebugAndRelease)
SET_LIBRARY_FROM_DEBUG_AND_RELEASE(LCMS)

message("LCMS ReleaseLibrary: ${LCMS_LIBRARY_RELEASE}")
message("LCMS Debug Library: ${LCMS_LIBRARY_DEBUG}")
message("LCMS Library: ${LCMS_LIBRARY}")

if (LCMS_LIBRARY AND LCMS_INCLUDE_DIR)
  set( LCMS_FOUND 1 )
  set( LCMS_LIBRARIES ${LCMS_LIBRARY} )
else (LCMS_LIBRARY AND LCMS_INCLUDE_DIR)
  set( LCMS_FOUND 0 )
endif (LCMS_LIBRARY AND LCMS_INCLUDE_DIR)

set(LCMS_FIND_QUIETLY 1)

if (LCMS_FOUND)
  if (NOT LCMS_FIND_QUIETLY)
    message(STATUS "Found LittleCMS: ${LCMS_LIBRARY}")
  endif (NOT LCMS_FIND_QUIETLY)
else (LCMS_FOUND)
  if (LCMS_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find LittleCMS")
  endif (LCMS_FIND_REQUIRED)
endif (LCMS_FOUND)
