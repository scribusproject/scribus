set(useshared)
if(WIN32)
	if(NOT DEFINED LIBPODOFO_SHARED)
		message("FATAL: Win32 users MUST set LIBPODOFO_SHARED")
		message("FATAL: Set -DLIBPODOFO_SHARED=0 if linking to a static library PoDoFo")
		message("FATAL: or -DLIBPODOFO_SHARED=1 if linking to a DLL build of PoDoFo")
		message(FATAL_ERROR "LIBPODOFO_SHARED unset on win32 build")
	else(NOT DEFINED LIBPODOFO_SHARED)
		if(LIBPODOFO_SHARED)
			set(useshared "-DUSING_SHARED_PODOFO")
		endif(LIBPODOFO_SHARED)
	endif(NOT DEFINED LIBPODOFO_SHARED)
endif(WIN32)

set(LIBPODOFO_FIND_QUIETLY ON)

find_package(PkgConfig QUIET)
if(DEFINED LIBPODOFO_DIR_PREFIX)
	set(ENV{PKG_CONFIG_PATH} "${LIBPODOFO_DIR_PREFIX}/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
endif()
pkg_search_module(libpodofo REQUIRED libpodofo podofo libpodofo-0)

if (DEFINED LIBPODOFO_DIR_PREFIX)
	find_path(LIBPODOFO_INCLUDE_DIR
		NAMES podofo/podofo.h
		PATHS
		"${LIBPODOFO_DIR_PREFIX}/include"
		"${LIBPODOFO_DIR_PREFIX}"
		PATH_SUFFIXES podofo
		NO_DEFAULT_PATH
	)

	find_library(LIBPODOFO_LIBRARY
		NAMES libpodofo podofo
		PATHS
		"${LIBPODOFO_DIR_PREFIX}/lib"
		"${LIBPODOFO_DIR_PREFIX}"
		NO_DEFAULT_PATH
	)
else()
	find_path(LIBPODOFO_INCLUDE_DIR
		NAMES podofo/podofo.h
		PATHS
		${libpodofo_INCLUDE_DIRS}
	)

	find_library(LIBPODOFO_LIBRARY
		NAMES libpodofo podofo
		PATHS
		${libpodofo_LIBRARY_DIRS}
	)
endif()


if(LIBPODOFO_INCLUDE_DIR AND LIBPODOFO_LIBRARY)
	if(NOT LIBPODOFO_CONFIG_H)
		#PoDoFo before 0.10.0
		if (EXISTS "${LIBPODOFO_INCLUDE_DIR}/podofo/base/podofo_config.h")
			set(LIBPODOFO_CONFIG_H "${LIBPODOFO_INCLUDE_DIR}/podofo/base/podofo_config.h" CACHE FILEPATH "Alternative place of podofo_config.h. PoDoFo <=0.10.0")
		endif()
		#PoDoFo 0.10.0 or higher
		if (EXISTS "${LIBPODOFO_INCLUDE_DIR}/podofo/auxiliary/podofo_config.h")
			set(LIBPODOFO_CONFIG_H "${LIBPODOFO_INCLUDE_DIR}/podofo/auxiliary/podofo_config.h" CACHE FILEPATH "Alternative place of podofo_config.h. PoDoFo >=0.10.0")
		endif()
	endif()

	file(STRINGS "${LIBPODOFO_CONFIG_H}" PODOFO_MAJOR_VER_LINE REGEX "^#define[ \t]+PODOFO_VERSION_MAJOR[ \t]+[0-9]+$")
	file(STRINGS "${LIBPODOFO_CONFIG_H}" PODOFO_MINOR_VER_LINE REGEX "^#define[ \t]+PODOFO_VERSION_MINOR[ \t]+[0-9]+$")
	file(STRINGS "${LIBPODOFO_CONFIG_H}" PODOFO_PATCH_VER_LINE REGEX "^#define[ \t]+PODOFO_VERSION_PATCH[ \t]+[0-9]+$")
	string(REGEX REPLACE "^#define[ \t]+PODOFO_VERSION_MAJOR[ \t]+([0-9]+)$" "\\1" PODOFO_MAJOR_VER "${PODOFO_MAJOR_VER_LINE}")
	string(REGEX REPLACE "^#define[ \t]+PODOFO_VERSION_MINOR[ \t]+([0-9]+)$" "\\1" PODOFO_MINOR_VER "${PODOFO_MINOR_VER_LINE}")
	string(REGEX REPLACE "^#define[ \t]+PODOFO_VERSION_PATCH[ \t]+([0-9]+)$" "\\1" PODOFO_PATCH_VER "${PODOFO_PATCH_VER_LINE}")
#	message(STATUS "PODOFO MAJOR VERSION ${PODOFO_MAJOR_VER}")
#	message(STATUS "PODOFO MINOR VERSION ${PODOFO_MINOR_VER}")
#	message(STATUS "PODOFO PATCH VERSION ${PODOFO_PATCH_VER}")
	set(LIBPODOFO_VERSION "${PODOFO_MAJOR_VER}.${PODOFO_MINOR_VER}.${PODOFO_PATCH_VER}" CACHE STRING "PoDoFo version string")
	if(LIBPODOFO_VERSION VERSION_GREATER "0.9.4")
		# podofo 0.9.5 use OpenSSL includes in its public headers, we need to know OpenSSL include dirs
		# (https://bugs.scribus.net/view.php?id=14838)
		find_package(OpenSSL QUIET)
		if (NOT OPENSSL_FOUND)
			find_package(OPENSSL)
		endif()
		if (OPENSSL_FOUND)
			message(STATUS "OpenSSL found OK for installed version of PoDoFo (>= 0.9.5) - Enabling support for PDF embedded in AI")
			set(LIBPODOFO_FOUND ON CACHE BOOL "Was libpodofo found")
		else()
			message(STATUS "OpenSSL NOT found for installed version of PoDoFo (>= 0.9.5) - Disabling support for PDF embedded in AI")
			unset(LIBPODOFO_FOUND)
		endif()
	else()
		message(STATUS "OpenSSL NOT required for installed version of PoDoFo (< 0.9.5) - Enabling support for PDF embedded in AI")
		set(LIBPODOFO_FOUND TRUE CACHE BOOL "Was libpodofo found")
	endif()
endif(LIBPODOFO_INCLUDE_DIR AND LIBPODOFO_LIBRARY)

set(LIBPODOFO_CFLAGS "${useshared}" CACHE STRING "Extra flags for compiling against PoDoFo")

if(NOT LIBPODOFO_FIND_QUIETLY)
	if(LIBPODOFO_INCLUDE_DIR)
		message("podofo/podofo.h: ${LIBPODOFO_INCLUDE_DIR}")
	else(LIBPODOFO_INCLUDE_DIR)
		message("podofo/podofo.h: not found")
	endif(LIBPODOFO_INCLUDE_DIR)
	if(LIBPODOFO_LIBRARY)
		message("PoDoFo lib: ${LIBPODOFO_LIBRARY}")
	else(LIBPODOFO_LIBRARY)
		message("PoDoFo lib: not found")
	endif(LIBPODOFO_LIBRARY)
	message("PoDoFo cflags: ${useshared}")
	message("PoDoFo Config File: ${LIBPODOFO_CONFIG_H}")
	message("PoDoFo Version: ${LIBPODOFO_VERSION}")
endif(NOT LIBPODOFO_FIND_QUIETLY)
