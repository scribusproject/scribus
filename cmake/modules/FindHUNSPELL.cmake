# - Try to find HUNSPELL
# Once done this will define
#
#  HUNSPELL_FOUND - System has HUNSPELL
#  HUNSPELL_INCLUDE_DIR - The HUNSPELL include directory
#  HUNSPELL_LIBRARIES - The libraries needed to use HUNSPELL
#  HUNSPELL_DEFINITIONS - Compiler switches required for using HUNSPELL

#Based on examples at http://www.vtk.org/Wiki/CMake:How_To_Find_Libraries

include(FindPkgConfig)
pkg_search_module(HUNSPELL REQUIRED hunspell)

find_path(HUNSPELL
  NAMES hunspell.h
  PATHS ${PKG_HUNSPELL_INCLUDE_DIRS} ${HUNSPELL_INCLUDE_DIRS} /usr/local/include /usr/include
  PATH_SUFFIXES hunspell
  NO_DEFAULT_PATH
)

find_library(HUNSPELL_LIBRARIES
  NAMES libhunspell
  PATHS ${PKG_HUNSPELL_LIBRARIES} ${HUNSPELL_LIBRARY_DIRS} /usr/local/lib /usr/lib
  NO_DEFAULT_PATH
)

string(REGEX MATCH "1\.([1-6])\.." HUNSPELL_MATCH ${HUNSPELL_VERSION})
#set(HUNSPELL_MINOR_VERSION ${CMAKE_MATCH_1})
if(CMAKE_MATCH_1 GREATER 4)
	set(HUNSPELL_NEWAPI 1)
	message(STATUS "New HUNSPELL API found")
endif()

#find_path(HUNSPELL_INCLUDE_DIR hunspell/hunspell.hxx )
#find_library(HUNSPELL_LIBRARIES NAMES hunspell-1.5 hunspell-1.4 hunspell-1.3 hunspell-1.2 PATHS /opt/local/lib /usr/local/lib /usr/lib )
#include(FindPackageHandleStandardArgs)
#FIND_PACKAGE_HANDLE_STANDARD_ARGS(HUNSPELL DEFAULT_MSG HUNSPELL_LIBRARIES HUNSPELL_INCLUDE_DIR )

mark_as_advanced(HUNSPELL_INCLUDE_DIR HUNSPELL_LIBRARIES)
