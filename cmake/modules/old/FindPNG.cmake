# - Find the native PNG includes and library
#
# This module defines
#  PNG_INCLUDE_DIR, where to find png.h, etc.
#  PNG_LIBRARIES, the libraries to link against to use PNG.
#  PNG_DEFINITIONS - You should ADD_DEFINITONS(${PNG_DEFINITIONS}) before compiling code that includes png library files.
#  PNG_FOUND, If false, do not try to use PNG.
# also defined, but not for general use are
#  PNG_LIBRARY, where to find the PNG library.
# None of the above will be defined unles zlib can be found.
# PNG depends on Zlib
INCLUDE(FindZLIB)

IF(ZLIB_FOUND)

  FIND_PATH(PNG_PNG_INCLUDE_DIR png.h
  /usr/local/include/libpng             # OpenBSD
  )

  # On win32, qt statically links to zlib and libpng, and exports their symbols.
  # We can just link to Qt to get zlib and libpng. We still require the user to
  # supply their own headers on the search path, but they can and should just
  # specify ${QTDIR}/src/3rdparty/zlib/include .
  # To use this, you must use FindQt before FindPng.
  IF(QT_QTCORE_LIBRARY AND USE_QT_ZLIB_PNGLIB)

    MESSAGE("Using libpng from qt")
    IF(NOT "${ZLIB_LIBRARY}" STREQUAL "${QT_QTCORE_LIBRARY}")
       MESSAGE("WARNING: We may be using an external zlib but qt's libpng")
    ENDIF(NOT "${ZLIB_LIBRARY}" STREQUAL "${QT_QTCORE_LIBRARY}")
    SET(PNG_LIBRARY_RELEASE ${QT_QTCORE_LIBRARY_RELEASE})
    SET(PNG_LIBRARY_DEBUG ${QT_QTCORE_LIBRARY_DEBUG})
    SET(PNG_LIBRARY ${QT_QTCORE_LIBRARY})

  ELSE(QT_QTCORE_LIBRARY AND USE_QT_ZLIB_PNGLIB)

    SET(PNG_NAMES_RELEASE ${PNG_NAMES_RELEASE} ${PNG_NAMES} png libpng png13 libpng13)
    FIND_LIBRARY(PNG_LIBRARY_RELEASE NAMES ${PNG_NAMES_RELEASE} )

    SET(PNG_NAMES_DEBUG ${PNG_NAMES} pngd libpngd png13d libpng13d)
    FIND_LIBRARY(PNG_LIBRARY_DEBUG NAMES ${PNG_NAMES_DEBUG} )

    INCLUDE(LibraryDebugAndRelease)
    SET_LIBRARY_FROM_DEBUG_AND_RELEASE(PNG)

  ENDIF(QT_QTCORE_LIBRARY AND USE_QT_ZLIB_PNGLIB)

  IF (PNG_LIBRARY AND PNG_PNG_INCLUDE_DIR)
      # png.h includes zlib.h. Sigh.
      SET(PNG_INCLUDE_DIR ${PNG_PNG_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} )
      IF(NOT USE_QT_ZLIB_PNGLIB)
        # libpng requires zlib unless we're linking to both in the form of Qt
        SET(PNG_LIBRARIES ${PNG_LIBRARY} ${ZLIB_LIBRARY})
      ENDIF(NOT USE_QT_ZLIB_PNGLIB)

      IF (CYGWIN)
        IF(BUILD_SHARED_LIBS)
           # No need to define PNG_USE_DLL here, because it's default for Cygwin.
        ELSE(BUILD_SHARED_LIBS)
          SET (PNG_DEFINITIONS -DPNG_STATIC)
        ENDIF(BUILD_SHARED_LIBS)
      ENDIF (CYGWIN)

  ENDIF (PNG_LIBRARY AND PNG_PNG_INCLUDE_DIR)

ENDIF(ZLIB_FOUND)

# handle the QUIETLY and REQUIRED arguments and set PNG_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(ScribusFindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PNG DEFAULT_MSG PNG_LIBRARY PNG_PNG_INCLUDE_DIR)

MESSAGE("Using PNG Library: ${PNG_LIBRARY}")

MARK_AS_ADVANCED(PNG_PNG_INCLUDE_DIR PNG_LIBRARY )
