# - Find HYPHEN library
# Find the native HYPHEN includes and library
# This module defines
#  HYPHEN_INCLUDE_DIR, where to find hyphen.h, etc.
#  HYPHEN_LIBRARIES, libraries to link against to use HYPHEN.
#  HYPHEN_FOUND, If false, do not try to use HYPHEN.
# also defined, but not for general use are
#  HYPHEN_LIBRARY, where to find the HYPHEN library.

find_path(HYPHEN_INCLUDE_DIR hyphen.h)

set(HYPHEN_NAMES_RELEASE ${HYPHEN_NAMES_RELEASE} ${HYPHEN_NAMES} hyphen libhyphen)
set(HYPHEN_NAMES_DEBUG ${HYPHEN_NAMES_DEBUG} hyphend libhyphend)

find_library(HYPHEN_LIBRARY_RELEASE NAMES ${HYPHEN_NAMES_RELEASE} )
find_library(HYPHEN_LIBRARY_DEBUG NAMES ${HYPHEN_NAMES_DEBUG} )

include(LibraryDebugAndRelease)
SET_LIBRARY_FROM_DEBUG_AND_RELEASE(HYPHEN)

# handle the QUIETLY and REQUIRED arguments and set HYPHEN_FOUND to TRUE if 
# all listed variables are TRUE
include(ScribusFindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(HYPHEN DEFAULT_MSG HYPHEN_LIBRARY HYPHEN_INCLUDE_DIR)

if(HYPHEN_FOUND)
  set( HYPHEN_LIBRARIES ${HYPHEN_LIBRARY} )
endif(HYPHEN_FOUND)

