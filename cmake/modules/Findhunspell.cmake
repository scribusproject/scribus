# - Try to find HUNSPELL
# Once done this will define
#
#  hunspell_FOUND - System has HUNSPELL
#  hunspell_INCLUDE_DIR - The HUNSPELL include directory
#  hunspell_LIBRARIES - The libraries needed to use HUNSPELL
#  hunspell_DEFINITIONS - Compiler switches required for using HUNSPELL

#Based on examples at http://www.vtk.org/Wiki/CMake:How_To_Find_Libraries

#include(FindPkgConfig)
find_package(PkgConfig QUIET)
pkg_search_module(hunspell REQUIRED hunspell)

find_path(hunspell
  NAMES hunspell.h
  PATHS ${PKG_hunspell_INCLUDE_DIRS} ${hunspell_INCLUDE_DIRS}
  PATH_SUFFIXES hunspell
  NO_DEFAULT_PATH
)

find_library(hunspell_LIBRARIES
  NAMES libhunspell
  PATHS ${PKG_hunspell_LIBRARIES} ${hunspell_LIBRARY_DIRS}
  NO_DEFAULT_PATH
)

string(REGEX MATCH "1\.([1-9])\.." hunspell_MATCH ${hunspell_VERSION})
#set(hunspell_MINOR_VERSION ${CMAKE_MATCH_1})
if(CMAKE_MATCH_1 GREATER 4)
	set(hunspell_NEWAPI ON)
	message(STATUS "New hunspell API found. hunspell version ${hunspell_VERSION}")
	message(STATUS "hunspell INCLUDE ${hunspell_INCLUDE_DIRS}")
	message(STATUS "hunspell LIBRARIES ${hunspell_LIBRARIES}")
endif()

mark_as_advanced(hunspell_INCLUDE_DIR hunspell_LIBRARIES)
