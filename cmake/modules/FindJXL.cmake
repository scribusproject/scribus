#
# Find the libjxl includes and library
#

find_path(JXL_INCLUDE_DIR version.h
  PATH_SUFFIXES jxl
)

find_library(JXL_LIBRARY_RELEASE
  NAMES jxl
  PATHS /usr/lib /usr/local/lib
)

find_library(JXL_THREADS_LIBRARY_RELEASE
  NAMES jxl_threads
  PATHS /usr/lib /usr/local/lib
)

SET(JXL_LIBRARY_DEBUG ${JXL_LIBRARY_RELEASE})
SET(JXL_THREADS_LIBRARY_DEBUG ${JXL_THREADS_LIBRARY_RELEASE})

include(LibraryDebugAndRelease)
SET_LIBRARY_FROM_DEBUG_AND_RELEASE(JXL)
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
		message("JPEG XL Libraries: ${JXL_LIBRARIES}")
		message("JPEG XL Threads Library: ${JXL_THREADS_LIBRARY}")
		message("JPEG XL Include: ${JXL_INCLUDE_DIR}")
	endif (JXL_FIND_QUIETLY)
else (JXL_FOUND)
	if (JXL_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find JPEG XL")
	endif (JXL_FIND_REQUIRED)
endif (JXL_FOUND)
