#
# Find the native LCMS includes and library
#

# This module defines
# LCMS_INCLUDE_DIR, where to find art*.h etc
# LCMS_LIBRARY, the libraries
# LCMS_FOUND, If false, do not try to use LCMS.
# LIBLCMS_LIBS, link information
# LIBLCMS_CFLAGS, cflags for include information


# INCLUDE(UsePkgConfig)

# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
# PKGCONFIG(lcms _lcmsIncDir _lcmsLinkDir _lcmsLinkFlags _lcmsCflags)

# SET(LCMS_LIBS ${_lcmsCflags})

FIND_PATH(LCMS_INCLUDE_DIR lcms.h
/usr/include
/usr/local/include
)

FIND_LIBRARY(LCMS_LIBRARY_RELEASE
  NAMES ${LCMS_NAMES_RELEASE} ${LCMS_NAMES} lcms liblcms lcmsdll
  PATHS /usr/lib /usr/local/lib
)

FIND_LIBRARY(LCMS_LIBRARY_DEBUG
  NAMES ${LCMS_NAMES_DEBUG} lcmsd liblcmsd lcmsdlld
  PATHS /usr/lib /usr/local/lib
)

INCLUDE(LibraryDebugAndRelease)
SET_LIBRARY_FROM_DEBUG_AND_RELEASE(LCMS)

MESSAGE("LCMS ReleaseLibrary: ${LCMS_LIBRARY_RELEASE}")
MESSAGE("LCMS Debug Library: ${LCMS_LIBRARY_DEBUG}")
MESSAGE("LCMS Library: ${LCMS_LIBRARY}")

IF (LCMS_LIBRARY AND LCMS_INCLUDE_DIR)
  SET( LCMS_FOUND 1 )
  SET( LCMS_LIBRARIES ${LCMS_LIBRARY} )
ELSE (LCMS_LIBRARY AND LCMS_INCLUDE_DIR)
  SET( LCMS_FOUND 0 )
ENDIF (LCMS_LIBRARY AND LCMS_INCLUDE_DIR)

SET(LCMS_FIND_QUIETLY 1)

IF (LCMS_FOUND)
  IF (NOT LCMS_FIND_QUIETLY)
    MESSAGE(STATUS "Found LittleCMS: ${LCMS_LIBRARY}")
  ENDIF (NOT LCMS_FIND_QUIETLY)
ELSE (LCMS_FOUND)
  IF (LCMS_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find LittleCMS")
  ENDIF (LCMS_FIND_REQUIRED)
ENDIF (LCMS_FOUND)
