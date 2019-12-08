# - Find zlib
# Find the native ZLIB includes and library
#
#  ZLIB_INCLUDE_DIR - where to find zlib.h, etc.
#  ZLIB_LIBRARIES   - List of libraries when using zlib.
#  ZLIB_FOUND       - True if zlib found.


if (ZLIB_INCLUDE_DIR)
	# Already in cache, be silent
	set(ZLIB_FIND_QUIETLY TRUE)
endif (ZLIB_INCLUDE_DIR)

find_path(ZLIB_INCLUDE_DIR zlib.h)

# On win32, qt statically links to zlib and libpng, and exports their symbols.
# We can just link to Qt to get zlib and libpng. We still require the user to
# supply their own headers on the search path, but they can and should just
# specify ${QTDIR}/src/3rdparty/zlib/include .
# To use this, you must use FindQt before FindZlib.
if(QT_QTCORE_LIBRARY AND USE_QT_ZLIB_PNGLIB)

	message("Using zlib from qt")
	set(ZLIB_LIBRARY_RELEASE ${QT_QTCORE_LIBRARY_RELEASE})
	set(ZLIB_LIBRARY_DEBUG ${QT_QTCORE_LIBRARY_DEBUG})
	set(ZLIB_LIBRARY ${QT_QTCORE_LIBRARY})

else(QT_QTCORE_LIBRARY AND USE_QT_ZLIB_PNGLIB)

	set(ZLIB_NAMES_RELEASE ${ZLIB_NAMES_RELEASE} ${ZLIB_NAMES} z zlib zdll zlib1)
	find_library(ZLIB_LIBRARY_RELEASE NAMES ${ZLIB_NAMES_RELEASE} )

	# Find a debug library if one exists and use that for debug builds.
	# This really only does anything for win32, but does no harm on other
	# platforms.
	set(ZLIB_NAMES_DEBUG ${ZLIB_NAMES_DEBUG} zlibd zlib1d)
	find_library(ZLIB_LIBRARY_DEBUG NAMES ${ZLIB_NAMES_DEBUG})

	include(LibraryDebugAndRelease)
	SET_LIBRARY_FROM_DEBUG_AND_RELEASE(ZLIB)

endif(QT_QTCORE_LIBRARY AND USE_QT_ZLIB_PNGLIB)

# handle the QUIETLY and REQUIRED arguments and set ZLIB_FOUND to TRUE if 
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ZLIB DEFAULT_MSG ZLIB_LIBRARY ZLIB_INCLUDE_DIR)

if(ZLIB_FOUND)
	set( ZLIB_LIBRARIES ${ZLIB_LIBRARY} )
else(ZLIB_FOUND)
	set( ZLIB_LIBRARIES )
endif(ZLIB_FOUND)

mark_as_advanced( ZLIB_LIBRARY ZLIB_INCLUDE_DIR )
