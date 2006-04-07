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

FIND_LIBRARY(LCMS_LIBRARY NAMES lcms liblcms
  PATHS /usr/lib /usr/local/lib
)

# MESSAGE(STATUS "lcmsinclude ${LCMS_INCLUDE_DIR}")

SET(LCMS_FIND_QUIETLY 1)

IF (LCMS_LIBRARY)
  IF (NOT LCMS_FIND_QUIETLY)
    MESSAGE(STATUS "Found LittleCMS: ${LCMS_LIBRARY}")
  ENDIF (NOT LCMS_FIND_QUIETLY)
  IF (LCMS_INCLUDE_DIR)
    SET( LCMS_FOUND "YES" )
    SET( LCMS_LIBRARIES ${LCMS_LIBRARY} )
#  	MESSAGE(STATUS ${LCMS_LIBS})
#  	MESSAGE(STATUS ${LCMS_CFLAGS})
  ENDIF (LCMS_INCLUDE_DIR)
ENDIF (LCMS_LIBRARY)