#
# Find the libjxl includes and library
#

find_package(PkgConfig QUIET)
pkg_search_module(libheif heif libheif>=1.23.7)

find_path(HEIF_INCLUDE_DIR
	NAMES heif_version.h
	PATHS ${libheif_INCLUDE_DIRS}
	PATH_SUFFIXES libheif
	NO_DEFAULT_PATH
)

find_library(libheif_LIBRARIES
	NAMES libheif heif
	PATHS ${PKG_libheif_LIBRARIES} ${libheif_LIBRARY_DIRS}
	NO_DEFAULT_PATH
)

find_library(HEIF_LIBRARY_RELEASE
	NAMES libheif heif
	PATHS ${libheif_LIBRARY_DIRS} /usr/lib /usr/local/lib /opt/local/lib
)

SET(HEIF_LIBRARY_DEBUG ${HEIF_LIBRARY_RELEASE})

include(LibraryDebugAndRelease)
SET_LIBRARY_FROM_DEBUG_AND_RELEASE(HEIF)

if (HEIF_LIBRARY AND HEIF_INCLUDE_DIR)
	set( HEIF_FOUND ON )
	set( HEIF_LIBRARIES ${HEIF_LIBRARY} )
else (HEIF_LIBRARY AND HEIF_INCLUDE_DIR)
	set( HEIF_FOUND OFF )
endif (HEIF_LIBRARY AND HEIF_INCLUDE_DIR)

if (HEIF_FOUND)
	if (HEIF_FIND_QUIETLY)
		message(STATUS "Found libheif: ${HEIF_LIBRARY}")
	else (HEIF_FIND_QUIETLY)
		message("libheif Release Library: ${HEIF_LIBRARY_RELEASE}")
		message("libheif Libraries: ${libheif_LIBRARIES}")
		message("libheif Include: ${HEIF_INCLUDE_DIR}")
	endif (HEIF_FIND_QUIETLY)
else (HEIF_FOUND)
	if (HEIF_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find libheif")
	endif (HEIF_FIND_REQUIRED)
endif (HEIF_FOUND)
