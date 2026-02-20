#
# Find the libjxl includes and library
#

find_package(PkgConfig QUIET)
pkg_search_module(libjxl libjxl>=0.11.2)
if (libjxl_FOUND)
	pkg_search_module(libjxl_cms libjxl_cms>=0.11.2)
	pkg_search_module(libjxl_threads libjxl_threads>=0.11.2)
endif(libjxl_FOUND)

find_path(JXL_INCLUDE_DIR
	NAMES version.h
	PATHS ${libjxl_INCLUDE_DIRS}
	PATH_SUFFIXES jxl
	NO_DEFAULT_PATH
)

find_path(JXL_CMS_INCLUDE_DIR
	NAMES version.h
	PATHS ${libjxl_cms_INCLUDE_DIRS}
	PATH_SUFFIXES jxl
	NO_DEFAULT_PATH
)

find_path(JXL_THREADS_INCLUDE_DIR
	NAMES version.h
	PATHS ${libjxl_threads_INCLUDE_DIRS}
	PATH_SUFFIXES jxl
	NO_DEFAULT_PATH
)

find_library(libjxl_LIBRARIES
	NAMES libjxl
	PATHS ${PKG_libjxl_LIBRARIES} ${libjxl_LIBRARY_DIRS}
	NO_DEFAULT_PATH
)

find_library(libjxl_cms_LIBRARIES
	NAMES libjxl_cms
	PATHS ${PKG_libjxl_cms_LIBRARIES} ${libjxl_cms_LIBRARY_DIRS}
	NO_DEFAULT_PATH
)

find_library(libjxl_threads_LIBRARIES
	NAMES libjxl_threads
	PATHS ${PKG_libjxl_threads_LIBRARIES} ${libjxl_threads_LIBRARY_DIRS}
	NO_DEFAULT_PATH
)


find_library(JXL_LIBRARY_RELEASE
  NAMES jxl
  PATHS ${libjxl_LIBRARY_DIRS} /usr/lib /usr/local/lib /opt/local/lib
)

find_library(JXL_CMS_LIBRARY_RELEASE
  NAMES jxl_cms
  PATHS ${libjxl_cmss_LIBRARY_DIRS} /usr/lib /usr/local/lib /opt/local/lib
)

find_library(JXL_THREADS_LIBRARY_RELEASE
  NAMES jxl_threads
  PATHS ${libjxl_threads_LIBRARY_DIRS} /usr/lib /usr/local/lib /opt/local/lib
)

SET(JXL_LIBRARY_DEBUG ${JXL_LIBRARY_RELEASE})
SET(JXL_CMS_LIBRARY_DEBUG ${JXL_CMS_LIBRARY_RELEASE})
SET(JXL_THREADS_LIBRARY_DEBUG ${JXL_THREADS_LIBRARY_RELEASE})

include(LibraryDebugAndRelease)
SET_LIBRARY_FROM_DEBUG_AND_RELEASE(JXL)
SET_LIBRARY_FROM_DEBUG_AND_RELEASE(JXL_CMS)
SET_LIBRARY_FROM_DEBUG_AND_RELEASE(JXL_THREADS)


if (JXL_LIBRARY AND JXL_THREADS_LIBRARY AND JXL_INCLUDE_DIR)
	set( JXL_FOUND ON )
	set( JXL_LIBRARIES ${JXL_LIBRARY} ${JXL_THREADS_LIBRARY} )
else (JXL_LIBRARY AND JXL_THREADS_LIBRARY AND JXL_INCLUDE_DIR)
	set( JXL_FOUND OFF )
endif (JXL_LIBRARY AND JXL_THREADS_LIBRARY AND JXL_INCLUDE_DIR)

#set(JXL_FIND_QUIETLY ON)

if (JXL_FOUND)
	if (JXL_FIND_QUIETLY)
		message(STATUS "Found JPEG XL: ${JXL_LIBRARY}")
	else (JXL_FIND_QUIETLY)
		message("JPEG XL Release Library: ${JXL_LIBRARY_RELEASE}")
		message("JPEG XL Libraries: ${libjxl_LIBRARIES}")
		message("JPEG XL Threads Library: ${JXL_THREADS_LIBRARY}")
		message("JPEG XL Include: ${JXL_INCLUDE_DIR}")
	endif (JXL_FIND_QUIETLY)
else (JXL_FOUND)
	if (JXL_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find JPEG XL")
	endif (JXL_FIND_REQUIRED)
endif (JXL_FOUND)
