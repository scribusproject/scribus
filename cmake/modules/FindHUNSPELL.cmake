# - Try to find HUNSPELL
# Once done this will define
#
#  HUNSPELL_FOUND - System has HUNSPELL
#  HUNSPELL_INCLUDE_DIR - The HUNSPELL include directory
#  HUNSPELL_LIBRARIES - The libraries needed to use HUNSPELL
#  HUNSPELL_DEFINITIONS - Compiler switches required for using HUNSPELL

#Based on examples at http://www.vtk.org/Wiki/CMake:How_To_Find_Libraries

FIND_PATH(HUNSPELL_INCLUDE_DIR hunspell/hunspell.hxx )
FIND_LIBRARY(HUNSPELL_LIBRARIES NAMES hunspell-1.3 hunspell-1.2 PATHS /opt/local/lib /usr/local/lib /usr/lib )

INCLUDE(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set HUNSPELL_FOUND to TRUE
# if all listed variables are TRUE

#FIND_PACKAGE(HUNSPELL QUIET NO_MODULE HINTS /opt/local/bin/hunspell)
#FIND_PACKAGE_HANDLE_STANDARD_ARGS(HUNSPELL  CONFIG_MODE)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(HUNSPELL DEFAULT_MSG HUNSPELL_LIBRARIES HUNSPELL_INCLUDE_DIR )

MARK_AS_ADVANCED(HUNSPELL_INCLUDE_DIR HUNSPELL_LIBRARIES)
