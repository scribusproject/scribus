# - Find zlib
# Find the native ZLIB includes and library
#
#  ZLIB_INCLUDE_DIR - where to find zlib.h, etc.
#  ZLIB_LIBRARIES   - List of libraries when using zlib.
#  ZLIB_FOUND       - True if zlib found.


IF (ZLIB_INCLUDE_DIR)
  # Already in cache, be silent
  SET(ZLIB_FIND_QUIETLY TRUE)
ENDIF (ZLIB_INCLUDE_DIR)

FIND_PATH(ZLIB_INCLUDE_DIR zlib.h)

# On win32, qt statically links to zlib and libpng, and exports their symbols.
# We can just link to Qt to get zlib and libpng. We still require the user to
# supply their own headers on the search path, but they can and should just
# specify ${QTDIR}/src/3rdparty/zlib/include .
# To use this, you must use FindQt before FindZlib.
IF(QT_QTCORE_LIBRARY AND USE_QT_ZLIB_PNGLIB)

  MESSAGE("Using zlib from qt")
  SET(ZLIB_LIBRARY_RELEASE ${QT_QTCORE_LIBRARY_RELEASE})
  SET(ZLIB_LIBRARY_DEBUG ${QT_QTCORE_LIBRARY_DEBUG})
  SET(ZLIB_LIBRARY ${QT_QTCORE_LIBRARY})

ELSE(QT_QTCORE_LIBRARY AND USE_QT_ZLIB_PNGLIB)

  SET(ZLIB_NAMES_RELEASE ${ZLIB_NAMES_RELEASE} ${ZLIB_NAMES} z zlib zdll zlib1)
  FIND_LIBRARY(ZLIB_LIBRARY_RELEASE NAMES ${ZLIB_NAMES_RELEASE} )

  # Find a debug library if one exists and use that for debug builds.
  # This really only does anything for win32, but does no harm on other
  # platforms.
  SET(ZLIB_NAMES_DEBUG ${ZLIB_NAMES_DEBUG} zlibd zlib1d)
  FIND_LIBRARY(ZLIB_LIBRARY_DEBUG NAMES ${ZLIB_NAMES_DEBUG})

  INCLUDE(LibraryDebugAndRelease)
  SET_LIBRARY_FROM_DEBUG_AND_RELEASE(ZLIB)

ENDIF(QT_QTCORE_LIBRARY AND USE_QT_ZLIB_PNGLIB)

# handle the QUIETLY and REQUIRED arguments and set ZLIB_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(ScribusFindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ZLIB DEFAULT_MSG ZLIB_LIBRARY ZLIB_INCLUDE_DIR)

IF(ZLIB_FOUND)
  SET( ZLIB_LIBRARIES ${ZLIB_LIBRARY} )
ELSE(ZLIB_FOUND)
  SET( ZLIB_LIBRARIES )
ENDIF(ZLIB_FOUND)

MARK_AS_ADVANCED( ZLIB_LIBRARY ZLIB_INCLUDE_DIR )
