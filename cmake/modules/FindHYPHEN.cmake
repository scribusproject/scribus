# - Find HYPHEN library
# Find the native HYPHEN includes and library
# This module defines
#  HYPHEN_INCLUDE_DIR, where to find hyphen.h, etc.
#  HYPHEN_LIBRARIES, libraries to link against to use HYPHEN.
#  HYPHEN_FOUND, If false, do not try to use HYPHEN.
# also defined, but not for general use are
#  HYPHEN_LIBRARY, where to find the HYPHEN library.

FIND_PATH(HYPHEN_INCLUDE_DIR hyphen.h)

SET(HYPHEN_NAMES_RELEASE ${HYPHEN_NAMES_RELEASE} ${HYPHEN_NAMES} hyphen libhyphen)
SET(HYPHEN_NAMES_DEBUG ${HYPHEN_NAMES_DEBUG} hyphend libhyphend)

FIND_LIBRARY(HYPHEN_LIBRARY_RELEASE NAMES ${HYPHEN_NAMES_RELEASE} )
FIND_LIBRARY(HYPHEN_LIBRARY_DEBUG NAMES ${HYPHEN_NAMES_DEBUG} )

INCLUDE(LibraryDebugAndRelease)
SET_LIBRARY_FROM_DEBUG_AND_RELEASE(HYPHEN)

# handle the QUIETLY and REQUIRED arguments and set HYPHEN_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(ScribusFindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(HYPHEN DEFAULT_MSG HYPHEN_LIBRARY HYPHEN_INCLUDE_DIR)

IF(HYPHEN_FOUND)
  SET( HYPHEN_LIBRARIES ${HYPHEN_LIBRARY} )
ENDIF(HYPHEN_FOUND)

